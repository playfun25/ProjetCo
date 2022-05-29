#pragma once
#include<cstdint>

#include<sensorstrategy.h>

/**
 * @brief 
 * Represente un capteur
 */
class Sensor
{

private:

    /**
     * @brief Temps restant avant le prochain reveil en millisecondes
     * 
     */
    int periodLeft;

    /**
     * @brief Temps de base entre chaque reveil
     * 
     */
    const int period;

    /**
     * @brief Type du capteur 
     * 
     */
    const int type;

    /**
     * @brief Strategie du capteur
     * 
     */
    SensorStrategy * strategy;

public:

    /**
     * @brief Construit un nouveau capteur
     * 
     * @param _period le temps que passe le capteur en mode sleep, utiliser le define
     * @param _type type de capteur, il faut utiliser le define
     * @param _strategy strategie que le capteur va utiliser, utiliser le define
     */
    Sensor(int _period,uint8_t _type, SensorStrategy * _strategy );

    /**
     * @brief Reduit le temps avant le prochain reveil
     * 
     * @param value valeur du temps a retire en millisecondes
     * @return int code erreur, -1 si l'operation n'est pas reussi
     */
    int decrementPeriod(int value);

    /**
     * @brief Remet le temps de reveil a sa valeur de base
     * 
     */
    void resetPeriod();

    /**
     * @brief Recupere la valeur du temps avant le prochain reveil
     * 
     * @return int la valeur du temps avant le prochain reveil en millisecondes
     */
    int getPeriodLeft() const;

    /**
     * @brief Recupere la mesure du capteur
     * 
     * @return int retourne la mesure du capteur
     */
    int getMesure();

    /**
     * @brief Recupere le type du capteur, correspond a un define
     * 
     * @return int retourne le type du capteur
     */
    int getType() const;

    /**
     * @brief Reveil le capteur
     * 
     * @return int retourne le code erreur de la strategie 
     */
    int wakeUp();

    /**
     * @brief Met le capteur en mode basse consommation
     * 
     * @return int retourne le code erreur de la strategie
     */
    int lowPower();

    /**
     * @brief Initialise le capteur
     * 
     * @return int retourne le code erreur de la strategie
     */
    int init();

};


