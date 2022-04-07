#pragma once
#include "sensorstrategy.h"

class tempSensorStrategy : public SensorStrategy
{
public:
    virtual ~tempSensorStrategy() {};
    int getMesure() override;
    int wakeUp() override;
    int init() override;
    int lowPower() override;
};

