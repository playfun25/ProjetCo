#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include "sensor.h"
#include "config.h"

// On verifie les capteurs actifs et non actif
// On include les strategies des capteurs actifs uniquement 
#ifdef TEMP_SENSOR_ACTVE
    #include "tempsensorstrategy.h"
#endif
#ifdef HUMID_SENSOR_ACTVE
    #include "humidsensorstrategy.h"
#endif
#ifdef CO2_SENSOR_ACTVE
    #include "co2sensorstrategy.h"
#endif
#ifdef LUX_SENSOR_ACTVE
    #include "luxSensorStrategy.h"
#endif
#ifdef PRESS_SENSOR_ACTVE
    #include "presssensorstrategy.h"
#endif
#ifdef eCO2_SENSOR_ACTVE
    #include "eco2sensorstrategy.h"
#endif
#ifdef TOVC_SENSOR_ACTVE
    #include "tovcsensorstrategy.h"
#endif
#ifdef UV_SENSOR_ACTVE
    #include "uvsensorstrategy.h"
#endif


/**
 * @brief Permettant de gerer l'integralite des capteurs actifs
 * 
 */
class SensorManager
{

private:
    /**
     * @brief Vecteur de tous les capteurs actifs
     * 
     */
    std::vector<Sensor*> * sensorVector;

    /** 
     * @brief Temps avant le prochain reveil
     * 
     */
    int nextSleepTime;

    /**
     * @brief Tri du vecteur de capteurs en fonction du temps avant le prochain reveil le plus petit
     * 
     */
    void sortSensorVector();

    /**
     * @brief Compare le temps restant avant le reveil entre deux capteurs
     * 
     * @param i1 premier capteur
     * @param i2 deuxieme capteur
     * @return true la comparaison a reussi
     * @return false la comparaison a echoue
     */
    static bool compareTimeLeft(Sensor* i1, Sensor* i2);

    /**
     * @brief Met a jour les temps restant avant le reveil des capteurs
     * 
     * @param timePassed temps a decrementer en millisecondes
     */
    void updateTimeLeft(int timePassed);

    /**
     * @brief Initialise les capteurs
     * 
     * @return int retourne 0 si tous c'est bien passe, retourne la derniere erreur capteur sinon
     */
    int initSensors();

public:

    /**
     * @brief Construit un nouveau manager de capteur
     * 
     */
    SensorManager();

    /**
     * @brief Reveil 
     * 
     * @param timePassed Temps a decrementer en millisecondes
     * @return int retourne 0 si tout c'est bien passe, sinon retourne la derniere erreur capteur
     */
    int wakeUp(int timePassed);

    /**
     * @brief Recupere le prochain temps avant le reveil
     * 
     * @return int retourne le prochain temps avant le reveil en millisecondes
     */
    int getNextSleepTime(){return nextSleepTime;};

};

