#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include "config.h"
#ifdef TEMP_SENSOR
    #include "tempsensorstrategy.h"
#endif
#ifdef HUMID_SENSOR
    #include "humidsensorstrategy.h"
#endif
#ifdef CO2_SENSOR
    #include "co2sensorstrategy.h"
#endif
#ifdef LUX_SENSOR
    #include "luxsensorstrategy.h"
#endif
#ifdef PRESS_SENSOR
    #include "presssensorstrategy.h"
#endif
#include "sensor.h"


class SensorManager
{
public:
    SensorManager();
    int wakeUp(int timePassed);
    int getNextSleepTime(){return nextSleepTime;};
private:
    std::vector<Sensor*> * sensorVector;
    void sortSensorVector();
    static bool compareTimeLeft(Sensor* i1, Sensor* i2);
    int findTimeLeft();
    void updateTimeLeft(int timePassed);
    int nextSleepTime;
    int initSensors();
};

