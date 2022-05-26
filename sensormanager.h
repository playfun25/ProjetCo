#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include "sensor.h"
#include "config.h"

#ifdef TEMP_SENSOR_ACTVE
    #include "tempsensorstrategy.h"
#endif
#ifdef HUMID_SENSOR_ACTVE
    #include "humidsensorstrategy.h"
#endif
#ifdef CO2_SENSOR_ACTVE
    #include "co2sensorstrategy.h"
#endif
#ifdef LUX_SENSOR_ACTVE
    #include "luxsensorstrategy.h"
#endif
#ifdef PRESS_SENSOR_ACTVE
    #include "presssensorstrategy.h"
#endif
#ifdef eCO2_SENSOR_ACTVE
    #include "eco2sensorstrategy.h"
#endif
#ifdef TOVC_SENSOR_ACTVE
    #include "tovcsensorstrategy.h"
#endif
#ifdef UV_SENSOR_ACTVE
    #include "uvsensorstrategy.h"
#endif



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

