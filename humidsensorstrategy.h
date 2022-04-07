#pragma once
#include "sensorstrategy.h"

class HumidSensorStrategy :  public SensorStrategy
{
public:
    HumidSensorStrategy();
    int getMesure() override;
    int wakeUp() override;
    int init() override;
    int lowPower() override;
};


