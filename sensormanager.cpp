#include "sensormanager.h"

Timer t;//mesur du temps de reveil

SensorManager::SensorManager()
{
    int ret =0;
    this->sensorVector = new std::vector<Sensor*>();


#ifdef PRESS_SENSOR
    sensorVector->push_back(new Sensor(LUX_SENSOR_PERIOD,LUX_SENSOR,new LUX_STRATEGIE));
#endif
#ifdef TEMP_SENSOR
    sensorVector->push_back(new Sensor(TEMP_SENSOR_PERIOD,TEMP_SENSOR,new TEMP_STRATEGIE));
#endif
#ifdef HUMID_SENSOR
    sensorVector->push_back(new Sensor(HUMID_SENSOR_PERIOD,HUMID_SENSOR,new HUMID_STRATEGIE));
#endif
#ifdef CO2_SENSOR
    sensorVector->push_back(new Sensor(CO2_SENSOR_PERIOD,CO2_SENSOR,new CO2_STRATEGIE));
#endif
#ifdef LUX_SENSOR
    sensorVector->push_back(new Sensor(PRESS_SENSOR_PERIOD,PRESS_SENSOR,new PRESS_STRATEGIE));
#endif
    ret += this->initSensors();
    if(ret){
        /* exeption
        */
    }
    this->sortSensorVector();
}

int SensorManager::wakeUp(int timePassed)
{
    int ret =0;
    this->updateTimeLeft(timePassed);
    if(this->sensorVector->at(0)->getPeriodLeft()<=0){
        /* get mesure
         */
    t.reset();
    t.start();
    if (this->sensorVector->at(0)->wakeUp()) return -1;
    t.stop();
    printf("temps de wake up : %f seconds\n", t.read());
    t.reset();
    t.start();
    if (this->sensorVector->at(0)->getMesure()) return -1;
    t.stop();
    printf("temps de get mesure : %f seconds\n", t.read());
     t.reset();
    t.start();
    if (this->sensorVector->at(0)->lowPower()) return -1;
    t.stop();
    printf("temps mise en low power : %f seconds\n", t.read());
        this->sensorVector->at(0)->resetPeriod();
        ret = this->sensorVector->at(0)->getType();
    }else{
        ret = -2;
    }
   
    this->sortSensorVector();
    nextSleepTime = this->sensorVector->at(0)->getPeriodLeft();
    return ret;
}

/*
int SensorManager::findNoTimeLeft()
{
    int i = 0;
    int retValue =-1;
    for(Sensor * it : *sensorVector){
        if(it->getPeriodLeft() <= 0){
            retValue = i;
        }
        i++;
    }
    return retValue;
}
*/
void SensorManager::updateTimeLeft(int timePassed)
{

    for(Sensor * it : *sensorVector){
        it->decrementPeriod(timePassed);
    }

}
int SensorManager::initSensors()
{   
    int ret = 0;
    int numsensor =1;
    for(Sensor * it : *sensorVector){
        numsensor++:
        t.reset();
         t.start();
         ret  += it->init();
         t.stop();
    printf("temps init capteur:%d : %f seconds\n",numsensor, t.read());
       
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

