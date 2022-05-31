#pragma once

#include "sensorstrategy.h"
#include "config.h"
#include "scd30.h"
#include "sensorslastvalue.h"

class Co2SensorStrategy : public SensorStrategy
{
public:
    Co2SensorStrategy() {};
    virtual ~Co2SensorStrategy() {};
    int getMesure() override {
        scd30 * scd = scd30::getInstance();
            scd->getReadyStatus();
        uint16_t redy = scd->scdSTR.ready;
        if(redy == scd30::SCDisReady) {
            uint8_t crcc = scd->readMeasurement();
            if(crcc != scd30::SCDnoERROR) return crcc;
            //printf("crd ok: %d\n",crcc);
            SensorsLastValue::GetInstance()->setCO2Value(scd->scdSTR.co2f);
            //SensorsLastValue::GetInstance()->setTempValue(scd->scdSTR.tempf);
            //SensorsLastValue::GetInstance()->setHumidValue(scd->scdSTR.humf);
            return 0;
        }
        return scd->scdSTR.ready;
    };
    int wakeUp() override
    {
        return 0;

    };
    int init() override 
    {
        int ret =0;
        scd30 * scd = scd30::getInstance();
        if(scd->getInitialise() != true){
        ret += scd->softReset();
        ThisThread::sleep_for(2000);
        ret += scd->getSerialNumber();
        
        ret += scd->setMeasInterval(CO2_SENSOR_PERIOD/1000);
        ret += scd->startMeasurement(0);
        scd->setInitialise();
        }
        return ret;
    };
    int lowPower() override
    {
         return 0;
    };
};
