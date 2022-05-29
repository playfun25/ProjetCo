#pragma once
#include "sensorstrategy.h"



/**
 * @brief Strategie des capteurs de temperature
 * 
 */
class tempSensorStrategy : public SensorStrategy
{
public:
    
    /**
     * @brief Construit la strategie
     * 
     */
    tempSensorStrategy();

    /**
     * @brief Recupere la mesure du capteur physique
     * 
     * @return int retourne la mesure du capteur
     */
    int getMesure() override;

    /**
     * @brief Reveil le capteur physique
     * 
     * @return int retourne 0 si tout c'est bien passe, retourne le code erreur du capteur sinon
     */
    int wakeUp() override;

    /**
     * @brief Initialise le capteur physique
     * 
     * @return retourne 0 si tout c'est bien passe, retourne le code erreur du capteur sinon
     */
    int init() override;

    /**
     * @brief Met le capteur physique en mode basse consomation
     * 
     * @return retourne 0 si tout c'est bien passe, retourne le code erreur du capteur sinon
     */
    int lowPower() override;

};

