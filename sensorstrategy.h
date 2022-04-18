#pragma once

#include "sensorslastvalue.h"

class SensorStrategy
{
public:
    virtual ~SensorStrategy() {}
    virtual int getMesure() = 0;
    virtual int init() = 0;
    virtual int wakeUp() = 0;
    virtual int lowPower() = 0;






};

