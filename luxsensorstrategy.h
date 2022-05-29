#pragma once
#include "sensorstrategy.h"
#include "sensorslastvalue.h"
#include "VEML7700.h"
#include "config.h"
#include "mbed.h"

class LuxSensorStrategy :  public SensorStrategy
{
    public:
    int getMesure( )
    {
        int res;
        uint16_t value;
        res += VEML7700::getInstance()->getALS(value);
        SensorsLastValue::GetInstance()->setLumiValue((float)(value/10));
        return res;
    };

    int init()
    {
        int res;
        LUX_POWER_LINE = 0;
        res += VEML7700::getInstance()->setALSConf(384);
        return res;
    };

    int wakeUp()
    {
        int res;
        LUX_POWER_LINE = 0;
        ThisThread::sleep_for(chrono::milliseconds(100));
        res += VEML7700::getInstance()->setALSConf(384);
        ThisThread::sleep_for(chrono::milliseconds(150));
        return res;
    };

    int lowPower()
    {
        LUX_POWER_LINE = 1;
        return 0;
    };
};