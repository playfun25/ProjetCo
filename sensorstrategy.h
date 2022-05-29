#pragma once

#include "sensorslastvalue.h"

/**
 * @brief Represente la strategie d'un capteur
 * 
 */
class SensorStrategy
{
public:
    /**
     * @brief Detruit la strategie
     * 
     */
    virtual ~SensorStrategy() {}

    /**
     * @brief Recupere la mesure 
     * 
     * @return int 
     */
    virtual int getMesure() = 0;

    /**
     * @brief Initialisation de la strategie
     * 
     * @return int 
     */
    virtual int init() = 0;

    /**
     * @brief Reveil du capteur 
     * 
     * @return int retourne le code erreur du capteur
     */
    virtual int wakeUp() = 0;

    /**
     * @brief Passe en mode basse consomation
     * 
     * @return int retourne le code erreur du capteur
     */
    virtual int lowPower() = 0;
};

