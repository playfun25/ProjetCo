#pragma once
#include "mbed.h"

DigitalOut Alim_l1(PA_7);
DigitalOut Alim_l2(PA_4);
DigitalOut Alim_l3(PA_9);

#define TEMP_SENSOR_ACTVE
#define TEMP_SENSOR 0x01
#define TEMP_SENSOR_PERIOD 2000
#define TEMP_STRATEGIE tempSensorStrategy()
//DigitalOut & TEMP_POWER_LINE = Alim_l1;

//#define HUMID_SENSOR_ACTVE
#define HUMID_SENSOR 0x02
#define HUMID_SENSOR_PERIOD 5000
#define HUMID_STRATEGIE HumidSensorStrategy()
//DigitalOut & HUMID_POWER_LINE = Alim_l1;

//#define LUX_SENSOR_ACTVE
#define LUX_SENSOR 0x03
#define LUX_SENSOR_PERIOD 11000
#define LUX_STRATEGIE LuxSensorStrategy()
//DigitalOut & LUX_POWER_LINE = Alim_l1;


//#define eCO2_SENSOR_ACTVE
#define eCO2_SENSOR 0x04
#define eCO2_SENSOR_PERIOD 5000
#define eCO2_STRATEGIE ECo2SensorStrategy()
//DigitalOut & ECO2_POWER_LINE = Alim_l1;

//#define TOVC_SENSOR_ACTVE
#define TOVC_SENSOR 0x05
#define TOVC_SENSOR_PERIOD 5000
#define TOVC_STRATEGIE TovcSensorStrategy()
//DigitalOut & TOVC_POWER_LINE = Alim_l1;

#define CO2_SENSOR_ACTVE
#define CO2_SENSOR 0x06
#define CO2_SENSOR_PERIOD 5000
#define CO2_STRATEGIE Co2SensorStrategy()
//DigitalOut & CO2_POWER_LINE = Alim_l1;

//#define PRESS_SENSOR_ACTVE
#define PRESS_SENSOR 0x07
#define PRESS_SENSOR_PERIOD 7000
#define PRESS_STRATEGIE PressSensorStrategy()
//DigitalOut & PRESS_POWER_LINE = Alim_l1;

//#define UV_SENSOR_ACTVE
#define UV_SENSOR 0x08
#define UV_SENSOR_PERIOD 7000
#define UV_STRATEGIE UvSensorStrategy()
//DigitalOut & UV_POWER_LINE = Alim_l1;




