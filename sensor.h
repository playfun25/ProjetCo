#pragma once
#include<cstdint>

#include<sensorstrategy.h>

/**
 * Classe qui represente un capteur
 * Cette classe fixe les methodes que tous les capteurs doivent implémenter
 */
class Sensor
{
public:
    /**
     * Construceur
     * \param period periode  
     * \param type 
     * \param strategy
     */
    Sensor(int _period,uint8_t _type, SensorStrategy * _strategy );
    int decrementPeriod(int value);
    void resetPeriod();
    int getPeriodLeft() const;
    int getMesure();
    int getType() const;
    int wakeUp();
    int lowPower();
    int init();

private:
    int periodLeft;
    const int period;
    const int type;
    SensorStrategy * strategy;
};


