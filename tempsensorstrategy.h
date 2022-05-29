#pragma once
#include "sensorstrategy.h"
#include "BME280.h"
#include "sensorslastvalue.h"
#include "config.h"

class tempSensorStrategy : public SensorStrategy
{
public:
    virtual ~tempSensorStrategy() {};
    int getMesure() override{
        // retreve the value after the measurment (values already calculated)
        float temp;
        int ret;
        // Measurment time = 1,25 + (2,3xt_OS) + (2,3xP_OS + 0,575) + (2,3xH_OS + 0,575) = 9,3ms -> 10ms
        ThisThread::sleep_for(10ms);
        ret = BME280::getInstance()->getTemperature(&temp);
        SensorsLastValue::GetInstance()->setTempValue(temp);
        return ret;
    };
    int wakeUp() override{
        TEMP_POWER_LINE = 0;
        BME280::getInstance()->initialize();
        BME280::getInstance()->setForcedMode();
        return 0;
    };
    int init() override{
        TEMP_POWER_LINE = 0;
        //init already done at the build of the object sensor
        //need add error reporting
        BME280::getInstance()->initialize();
        BME280::getInstance()->setForcedMode();
         return 0;
    };
    int lowPower() override{
         TEMP_POWER_LINE = 1;
         //already done in the performReading method (after the measurments)
        return 0;
    };
};