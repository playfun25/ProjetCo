#pragma once
#include<cstdint>

#include"sensorstrategy.h"

class Sensor
{
public:
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


