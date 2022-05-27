#pragma once
#include "sensorstrategy.h"
#include "SI1145.h"
#include "config.h"

class UvSensorStrategy :  public SensorStrategy
{
public:
    int getMesure() override{
        // retreve the value after the measurment (values already calculated)
        int ret;
        uint16_t temp;
        ret += SI1145::getInstance()->getUV(temp);
        ret += SI1145::getInstance()->getUV(temp);
         SensorsLastValue::GetInstance()->setUVValue((float)temp);
        return 0;
    };
    int wakeUp() override{
        int ret;
        UV_POWER_LINE = 0;
        ret += SI1145::getInstance()->initalize();
        return ret ;
    };
    int init() override{
    // iitialize the sensor 
    // true = 0K / false = not good
        int ret;
        UV_POWER_LINE = 0;
        ret += SI1145::getInstance()->initalize();
        return ret ;
    };
    int lowPower() override{
        UV_POWER_LINE = 1;
        return 0;
    };
};



