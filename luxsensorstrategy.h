#pragma once

#include "sensorstrategy.h"



class LuxSensorStrategy: public SensorStrategy
{
public:
    LuxSensorStrategy();
    int getMesure() override;
    int wakeUp() override;
    int init() override;
    int lowPower() override;
};