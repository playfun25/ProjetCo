
#include <chrono>
#include <cstdio>
#include <mbed.h>
#include <stdio.h>
#include <string>

#include "Callback.h"
#include "DigitalOut.h"
#include "ThisThread.h"
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
uint8_t rx_buffer[30] ;

// Flag pour mettre les thread en attente
constexpr uint32_t CONNECTED_TEST_FLAG = 0x00000002;
constexpr uint32_t DISCONNECTED_TEST_FLAG = 0x00000004;

//
constexpr uint32_t WATCHDOG_TICKER_PERDIOD = 1000;

constexpr uint16_t TIME_BUTTON_RESET_PRESSED = 3000;
constexpr uint16_t TIME_BUTTON_STATE_PRESSED = 10000;

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

//ID du dernier capteur ayant prit la mesure
int typeCapteur = 0;


uint32_t watchdogTicker;
I2C i2c(I2C_SDA,I2C_SCL);

Ticker watchdogTimer;

Ticker timePushButton;
Thread thread_connected;
Thread thread_disconnected;

DigitalOut indicator(LED3);

InterruptIn bpResetLora(PA_5);

static void send_message();


/**
 * Un watchdog cutomise car le watchdog integre sur la carte ne permet pas compter plus que 30 secondes
 */
void customWatchDog(void)
{
    watchdogTicker += 1;

    if(watchdogTicker >= TIMEOUT_WATCHDOG_APP) {
        NVIC_SystemReset();
    }

}


/*
 * Execute lorsqu'un test par appui sur bouton poussoir est demande
 * Et que la connexion est etablie
 */
void connectedIdicator()
{
    while(1)
    {
        // On attend le flag qui est set lors de l'appui sur le bouton etat lora
        ThisThread::flags_wait_any(CONNECTED_TEST_FLAG);
        indicator = 1;
        ThisThread::sleep_for(chrono::milliseconds(3000));
        indicator = 0;
    }
}


/*
 * Execute lorsqu'un test par appui sur bouton poussoir est demande
 * Et que la connexion n'est pas etablie
 */
void disconnectIdicator()
{
    while(1)
    {
        // On attend le flag qui est set lors de l'appui sur le bouton etat lora
        ThisThread::flags_wait_any(DISCONNECTED_TEST_FLAG);
        for(int i =0;i<=15;i++)
        {
            indicator = 1;
            ThisThread::sleep_for(chrono::milliseconds(100));
            indicator = 0;
            ThisThread::sleep_for(chrono::milliseconds(100));
        }
    }
}

/**
 * Appele apres x secondes d'appui sur le bouton etat lora
 */
void connectionTest()
{
    if(connected) {
        thread_connected.flags_set(CONNECTED_TEST_FLAG);
        
    } else {
        thread_disconnected.flags_set(DISCONNECTED_TEST_FLAG);
        
    }
}


/**
 * Appele apres x secondes d'appui sur le bouton reset lora
 */
void resetLora(void)
{
    NVIC_SystemReset();
}


/**
 * Capte un front montant sur le bouton reset lora
 */
void handlerRiseBpReset()
{
    timePushButton.detach();
}

/**
 * Capte un front descendant sur le bouton reset lora
 */
void handlerFallBpReset()
{
    timePushButton.attach(&resetLora, chrono::milliseconds(TIME_BUTTON_RESET_PRESSED));
}


/**
 * Capte un front montant sur le bouton etat lora
 */
void handlerRiseBpTest()
{
    timePushButton.detach();
}


/**
 * Capte un front descendant sur le bouton etat lora
 */
void handlerFallBpTest()
{
    timePushButton.attach(&connectionTest, TIME_BUTTON_STATE_PRESSED);
}


/**
 * Affiche la mesure
 */
void printMesure(int type)
{
    switch(type) {
        case TEMP_SENSOR:
            printf("temperature : %.2f \n",SensorsLastValue::GetInstance()->getTempValue() );
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
            printf("Luminosite : %.2f \n",SensorsLastValue::GetInstance()->getLumiValue() );
            break;
        case eCO2_SENSOR:
            printf("eCO2  : %.2f \n",SensorsLastValue::GetInstance()->geteCO2Value() );
            break;
        case TOVC_SENSOR:
            printf("Tovc : %.2f \n",SensorsLastValue::GetInstance()->getCOVValue() );
            break;
        case UV_SENSOR:
            printf("UV : %.2f \n",SensorsLastValue::GetInstance()->getUVValue() );
            break;
    }
}


/**
 * Point d'entree de l'application
 */
int main(void)
{
    // Temps d'attente mis a jour a chaque mesure 
    int sleepTime = 0;
    
    // Compte les tick, peut etre reset en le fixant à 0
    watchdogTicker = 0;
    
    // Lance le ticker toutes les x milisecondes pour le custom watchdog
    watchdogTimer.attach(&customWatchDog, chrono::milliseconds(WATCHDOG_TICKER_PERDIOD));
    
    // Gestion du bouton raset lora
    bpResetLora.mode(PullUp);
    bpResetLora.rise(&handlerRiseBpTest);
    bpResetLora.fall(&handlerFallBpTest);
    
    // Lance les threads d'affichage de l'etat de la communication lora 
    thread_connected.start(connectedIdicator);
    thread_disconnected.start(disconnectIdicator);
    
    SensorManager sensor;
    
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

    if (retcode == LORAWAN_STATUS_OK || retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS) {

    } else {
        printf("\r\n Connection error, code = %d \r\n", retcode);
        return -1;
    }

    printf("\r\n Connection - In Progress ...\r\n");

    // On donne du temps a la queue d'evenement pour qu'elle realise les taches en cours
    // Tant qu'on est pas connecte
    do {
        ev_queue.dispatch_for(std::chrono::milliseconds(CONNECTION_TEMPO));
    } while(connected == false);
    //resetwatchdog
    watchdogTicker = 0;

    // Boucle du programme principal
    while(1) {

        frameSent = false;

        // On realise la gestion capteur
        // reveil --> recuperation d'une nouvelle valeur --> repos
        typeCapteur = sensor.wakeUp(sleepTime);
        sleepTime = sensor.getNextSleepTime();
        printMesure(typeCapteur);
        
        // On reset le watchdog
        watchdogTicker = 0;
        
        // Ajout de l'evenement a la queue
        ev_queue.call_in(1, send_message);
        
        // Tant que la queue n'a pas fini de gerer l'evenement
        // On alloue du temps a la queue
        while(frameSent == false) {
            ev_queue.dispatch_for(events::EventQueue::duration(SEND_TEMPO));
        }
        
        // On reset le watchdog
        watchdogTicker = 0;
        
        // Le watchdog arret de coompter le temps qu'on dort
        watchdogTimer.detach();
        ThisThread::sleep_for(chrono::milliseconds(sleepTime));
        
        // Lors du reveil, le watchdog recommence a compter
        watchdogTimer.attach(&customWatchDog, 1s);
    }

    return 0;
}


/**
 * Envoi d'un message au serveur LoRa
 */
static void send_message()
{
    int16_t retcode;
    int32_t sensor_value;
    LoraFrame* frame;
    std::string frameString;

    // Creation de la trame
    frame = new LoraFrame();

    // Ajout des valeurs necessaires a la trame
    switch(typeCapteur) {
        case TEMP_SENSOR:
            frame->addData(typeCapteur,SensorsLastValue::GetInstance()->getTempValue());
            break;
        case HUMID_SENSOR:
            frame->addData(typeCapteur,SensorsLastValue::GetInstance()->getHumidValue() );
            break;
        case PRESS_SENSOR:
            frame->addData(typeCapteur,SensorsLastValue::GetInstance()->getpressValue() );
            break;
        case CO2_SENSOR:
            frame->addData(typeCapteur,SensorsLastValue::GetInstance()->getCO2Value() );
            break;
        case LUX_SENSOR:
            frame->addData(typeCapteur,SensorsLastValue::GetInstance()->getLumiValue() );
            break;
        case eCO2_SENSOR:
            frame->addData(typeCapteur,SensorsLastValue::GetInstance()->geteCO2Value() );
            break;
        case TOVC_SENSOR:
            frame->addData(typeCapteur,SensorsLastValue::GetInstance()->getCOVValue() );
            break;
        case UV_SENSOR:
            frame->addData(typeCapteur,SensorsLastValue::GetInstance()->getUVValue() );
            break;
    }
    
    // Transforme la trame en string
    for(uint8_t t : frame->getFrame()) {
        frameString.append(1,t);
    }
    
    // Affichage de la trame en hexa
    printf("frame:");
    for (int i = 0; frameString[i] != '\0'; i++) {
        printf(" %x", frameString[i]);
    }
    printf("\n");
    
    // Envoi de la trame
    retcode = lorawan.send(MBED_CONF_LORA_APP_PORT, (uint8_t *)frameString.c_str(), frameString.length(),
                           MSG_UNCONFIRMED_FLAG);

    printf("\r\n %d bytes prepares a etre transmis \r\n", retcode);
    memset(tx_buffer, 0, sizeof(tx_buffer));
}


/**
 * Methode appele lors de la reception d'un message
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
 * Gestionnaire d'evenement, appele lors de la reception d'un evenement LoRa
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
            NVIC_SystemReset();
            connected = false;
            break;

        case TX_DONE:
            printf("\r\n Le message a ete envoye  \r\n");
            frameSent = true;
            break;

        case TX_TIMEOUT:
            NVIC_SystemReset();
            break;
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
            NVIC_SystemReset();
            break;

        case RX_ERROR:
            printf("\r\n Erreur de reception - Code = %d \r\n", event);
            break;

        case JOIN_FAILURE:
            printf("\r\n OTAA Failed - Check Keys \r\n");
            NVIC_SystemReset();

            break;

        case UPLINK_REQUIRED:
            printf("\r\n Uplink required by NS \r\n");
            NVIC_SystemReset();
            break;
        default:
            MBED_ASSERT("Unknown Event");
            NVIC_SystemReset();
    }
}
