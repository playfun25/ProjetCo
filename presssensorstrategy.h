#pragma once
#include "sensorstrategy.h"

class PressSensorStrategy :  public SensorStrategy
{
public:
    PressSensorStrategy();
    int getMesure() override;
    int wakeUp() override;
    int init() override;
    int lowPower() override;
};


