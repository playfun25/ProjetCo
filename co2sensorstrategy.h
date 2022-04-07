#pragma once

#include "sensorstrategy.h"


class Co2SensorStrategy : public SensorStrategy
{
public:
    Co2SensorStrategy();
    int getMesure() override;
    int wakeUp() override;
    int init() override;
    int lowPower() override;
};

