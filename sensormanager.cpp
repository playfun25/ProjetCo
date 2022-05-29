#include "sensormanager.h"
#include "config.h"


DigitalOut  Alim_l1(ALIM_L1);
DigitalOut  Alim_l2(ALIM_L2);
DigitalOut  Alim_l3(ALIM_L3);

SensorManager::SensorManager()
{
    int ret =0;

    Alim_l1 =0;
    Alim_l2 =0;
    Alim_l3 =0;

    this->sensorVector = new std::vector<Sensor*>();
    
    #ifdef UV_SENSOR_ACTVE
        sensorVector->push_back(new Sensor(UV_SENSOR_PERIOD,UV_SENSOR,new UV_STRATEGIE));
    #endif

    #ifdef LUX_SENSOR_ACTVE
        sensorVector->push_back(new Sensor(LUX_SENSOR_PERIOD,LUX_SENSOR,new LUX_STRATEGIE));
    #endif
    #ifdef TEMP_SENSOR_ACTVE
        sensorVector->push_back(new Sensor(TEMP_SENSOR_PERIOD,TEMP_SENSOR,new TEMP_STRATEGIE));
    #endif
    #ifdef HUMID_SENSOR_ACTVE
        sensorVector->push_back(new Sensor(HUMID_SENSOR_PERIOD,HUMID_SENSOR,new HUMID_STRATEGIE));
    #endif
    #ifdef CO2_SENSOR_ACTVE
        sensorVector->push_back(new Sensor(CO2_SENSOR_PERIOD,CO2_SENSOR,new CO2_STRATEGIE));
    #endif
    #ifdef PRESS_SENSOR_ACTVE
        sensorVector->push_back(new Sensor(PRESS_SENSOR_PERIOD,PRESS_SENSOR,new PRESS_STRATEGIE));
    #endif
    #ifdef eCO2_SENSOR_ACTVE
        sensorVector->push_back(new Sensor(ECO2_SENSOR_PERIOD,ECO2_SENSOR,new ECO2_STRATEGIE));
    #endif
    #ifdef TOVC_SENSOR_ACTVE
        sensorVector->push_back(new Sensor(TOVC_SENSOR_PERIOD,TOVC_SENSOR,new TOVC_STRATEGIE));
    #endif

    ret += this->initSensors();

    if(ret)
    {
        // Gestion de l'erreur
    }

    this->sortSensorVector();
}

int SensorManager::wakeUp(int timePassed)
{
    int ret =0;

    this->updateTimeLeft(timePassed);

    // Si le temps avant la prochaine mesure est ecoule
    if(this->sensorVector->at(0)->getPeriodLeft()<=0)
    {
        if (this->sensorVector->at(0)->wakeUp())
        {
            //return -1;
        }
        if (this->sensorVector->at(0)->getMesure())
        {
            //return -2;
        }
        if (this->sensorVector->at(0)->lowPower())
        {
            //return -3;
        }

        this->sensorVector->at(0)->resetPeriod();
        ret = this->sensorVector->at(0)->getType();
    }
    else
    {
        ret = -4;
    }
   
    this->sortSensorVector();
    nextSleepTime = this->sensorVector->at(0)->getPeriodLeft();

    return ret;
}

void SensorManager::updateTimeLeft(int timePassed)
{
    for(Sensor *it : *sensorVector)
    {
        it->decrementPeriod(timePassed);
    }
}

int SensorManager::initSensors()
{   
    int ret = 0;

    for(Sensor * it : *sensorVector)
    {
       ret  += it->init();
       ret  += it->lowPower();
    }

    return ret;
}

void SensorManager::sortSensorVector()
{
    std::sort(sensorVector->begin(),sensorVector->end(),this->compareTimeLeft);
}

bool SensorManager::compareTimeLeft(Sensor *i1, Sensor *i2)
{
    return (i1->getPeriodLeft() < i2->getPeriodLeft());
}

