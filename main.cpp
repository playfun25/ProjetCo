#include <chrono>
#include <cstdio>
#include <mbed.h>
#include <stdio.h>
#include <string>

#include "Callback.h"
#include "DigitalOut.h"
#include "ThisThread.h"
#include "config.h"
#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"
#include "sensormanager.h"

#include "trace_helper.h"
#include "lora_radio_helper.hpp"
#include "LoraFrame.hpp"

using namespace events;

// Taille maximum des messages LoRa
uint8_t tx_buffer[30];
uint8_t rx_buffer[30];


//constexpr uint16_t TX_TIMER = 10000;

//
constexpr uint16_t CONNECTION_TEMPO = 1000;
constexpr uint16_t SEND_TEMPO = 1000;


// Nombre maximum d'evenement dans la queue
constexpr uint8_t MAX_NUMBER_OF_EVENTS = 10;


 // Nombre maxium d'essai pour la connexion au reseau LoRa
constexpr uint8_t  CONFIRMED_MSG_RETRY_COUNTER = 3;


// Queue d'evenement, doit fonctionner dans le thread principal
static EventQueue ev_queue(MAX_NUMBER_OF_EVENTS *EVENTS_EVENT_SIZE);


// Permet de gerer les differents evenement lora
static void lora_event_handler(lorawan_event_t event);


// Construit une interface lora en passant un objet radio depuis le lora_radio_helper
static LoRaWANInterface lorawan(radio);

// definit le callback de l'application lora
static lorawan_app_callbacks_t callbacks;


// Mis a jour lors de preparation d'un message et d'un envoi effectif
bool frameSent = false;


// Mis a jour lors de la connexion et de la deconnexion
bool connected = false;


int timeBZ = 0; // temporaire temps de dodo pour capteur 



static void send_message();




// Affiche la mesure 
void printMesure(int type)
{   
    switch(type){
        case TEMP_SENSOR:
        printf("température : %.2f \n",SensorsLastValue::GetInstance()->getTempValue() );
        break;
        case HUMID_SENSOR: 
        printf("Humidité : %.2f \n",SensorsLastValue::GetInstance()->getHumidValue() );
        break;
        case PRESS_SENSOR: 
        printf("Pression : %.2f \n",SensorsLastValue::GetInstance()->getpressValue() );
        break;
        case CO2_SENSOR: 
        printf("Co2 : %.2f \n",SensorsLastValue::GetInstance()->getCO2Value() );
        break;
        case LUX_SENSOR:
        printf("Luminausité : %.2f \n",SensorsLastValue::GetInstance()->getLumiValue() ); 
        break;
    }
}



/**
 * Point d'entrée de l'application
 */
int main(void)
{
    SensorManager sensor;
    //DigitalOut(sb27);
    int typeCapteur = 0;
    
    // Permet d'afficher les traces
    setup_trace();

    // Status du protocole LoRaWAN
    lorawan_status_t retcode;

    // Initialisation de la stack LoRaWAN
    if (lorawan.initialize(&ev_queue) != LORAWAN_STATUS_OK) {
        printf("\r\n Echec de l'initialisation LoRaWAN \r\n");
        return -1;
    }

    printf("\r\n Succes de l'initialisation LoRaWAN \r\n");

    callbacks.events = mbed::callback(lora_event_handler);
    lorawan.add_app_callbacks(&callbacks);

    // Set number of retries in case of CONFIRMED messages
    if (lorawan.set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER)
            != LORAWAN_STATUS_OK) {
        printf("\r\n set_confirmed_msg_retries failed! \r\n\r\n");
        return -1;
    }

    printf("\r\n CONFIRMED message retries : %d \r\n",
           CONFIRMED_MSG_RETRY_COUNTER);

    // Enable adaptive data rate
    if (lorawan.enable_adaptive_datarate() != LORAWAN_STATUS_OK) {
        printf("\r\n enable_adaptive_datarate failed! \r\n");
        return -1;
    }

    printf("\r\n Adaptive data  rate (ADR) - Enabled \r\n");

    retcode = lorawan.connect();

    if (retcode == LORAWAN_STATUS_OK || retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS) 
    {

    } 
    else 
    {
        printf("\r\n Connection error, code = %d \r\n", retcode);
        return -1;
    }

    printf("\r\n Connection - In Progress ...\r\n");

    // On donne du temps a la queue d'evenement pour qu'elle realise les taches en cours
    // Tant qu'on est pas connecte
    do
    {
        ev_queue.dispatch_for(std::chrono::milliseconds(CONNECTION_TEMPO));
    } while(connected == false);


    // Boucle du programme principal
    while(1)
    {
        frameSent = false;

        // On realise la gestion capteur
        // reveil --> recuperation d'une nouvelle valeur --> repos
        typeCapteur = sensor.wakeUp(timeBZ);
        timeBZ = sensor.getNextSleepTime();
        printMesure(typeCapteur);
        ThisThread::sleep_for(chrono::milliseconds(timeBZ));

        ev_queue.call_in(1, send_message);
        while(frameSent == false)
        {
            ev_queue.dispatch_for(events::EventQueue::duration(SEND_TEMPO));
        }
    }

    return 0;
}

/**
 * Sends a message to the Network Server
 */
static void send_message()
{
    uint16_t packet_len;
    int16_t retcode;
    int32_t sensor_value;
    LoraFrame* frame;
    std::string frameString;

    // Creation de la trame
    frame = new LoraFrame();

    // Ajoute les donnees des capteurs necessaire a la trame 
    frame->addData(TEMP_SENSOR, 10);

    // Recupere la trame sous forme d'une chaine de character
    frameString = std::string(frame->getFrame().begin(), frame->getFrame().end());

    // Recupere la taille de la trame
    packet_len = sprintf((char *) tx_buffer, frameString.c_str());

    // Envoi de la trame
    retcode = lorawan.send(MBED_CONF_LORA_APP_PORT, tx_buffer, packet_len,
                           MSG_UNCONFIRMED_FLAG);

    printf("\r\n %d bytes preparé à être transmis \r\n", retcode);
    memset(tx_buffer, 0, sizeof(tx_buffer));
}


/**
 * Methode appelé lors de la reception d'un message 
 */
static void receive_message()
{
    uint8_t port;
    int flags;
    int16_t retcode = lorawan.receive(rx_buffer, sizeof(rx_buffer), port, flags);

    if (retcode < 0) {
        printf("\r\n receive() - Error code %d \r\n", retcode);
        return;
    }

    printf(" RX Data on port %u (%d bytes): ", port, retcode);
    for (uint8_t i = 0; i < retcode; i++) {
        printf("%02x ", rx_buffer[i]);
    }
    printf("\r\n");

    memset(rx_buffer, 0, sizeof(rx_buffer));
}



/**
 * Event handler
 */
static void lora_event_handler(lorawan_event_t event)
{
    switch (event) {
        case CONNECTED:
            printf("\r\n Connection - Successful \r\n");
            connected = true;
            break;

        case DISCONNECTED:
            ev_queue.break_dispatch();
            printf("\r\n Deconnecte avec succes \r\n");
            connected = false;
            break;

        case TX_DONE:
            printf("\r\n Le message a ete envoye  \r\n");
            frameSent = true;
            break;

        case TX_TIMEOUT:
        case TX_ERROR:
        case TX_CRYPTO_ERROR:

        case TX_SCHEDULING_ERROR:
            printf("\r\n Erreur de transmission - EventCode = %d \r\n", event);
            break;

        case RX_DONE:
            printf("\r\n Reception d'un message du serveur distant \r\n");
            receive_message();
            break;

        case RX_TIMEOUT:

        case RX_ERROR:
            printf("\r\n Erreur de reception - Code = %d \r\n", event);
            break;

        case JOIN_FAILURE:
            printf("\r\n OTAA Failed - Check Keys \r\n");
            break;
            
        case UPLINK_REQUIRED:
            printf("\r\n Uplink required by NS \r\n");
            break;
        default:
            MBED_ASSERT("Unknown Event");
    }
}
