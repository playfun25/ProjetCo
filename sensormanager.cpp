#include "sensormanager.h"



SensorManager::SensorManager()
{
    this->sensorVector = new std::vector<Sensor*>();



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
#ifdef PRESS_SENSOR
    sensorVector->push_back(new Sensor(LUX_SENSOR_PERIOD,LUX_SENSOR,new LUX_STRATEGIE));
#endif
    this->sortSensorVector();
}

int SensorManager::wakeUp(int timePassed)
{
    this->updateTimeLeft(timePassed);
    if(this->sensorVector->at(0)->getPeriodLeft()<=0){
        /* get mesure
         */

        std::cout <<"mesure capteur: " << this->sensorVector->at(0)->getType()<< "mesure égale : "<< this->sensorVector->at(0)->getMesure() << '\n';
        this->sensorVector->at(0)->resetPeriod();
    }

    this->sortSensorVector();
    return this->sensorVector->at(0)->getPeriodLeft();
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

void SensorManager::sortSensorVector()
{
    std::sort(sensorVector->begin(),sensorVector->end(),this->compareTimeLeft);
}

bool SensorManager::compareTimeLeft(Sensor *i1, Sensor *i2)
{
    return (i1->getPeriodLeft() < i2->getPeriodLeft());
}

