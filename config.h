#pragma once


#define TEMP_SENSOR 0x01
#define TEMP_SENSOR_PERIOD 2000
#define TEMP_STRATEGIE tempSensorStrategy()


#define HUMID_SENSOR 0x02
#define HUMID_SENSOR_PERIOD 3000
#define HUMID_STRATEGIE HumidSensorStrategy()

#define CO2_SENSOR 0x03
#define CO2_SENSOR_PERIOD 5000
#define CO2_STRATEGIE Co2SensorStrategy()

#define PRESS_SENSOR 0x04
#define PRESS_SENSOR_PERIOD 7000
#define PRESS_STRATEGIE PressSensorStrategy()

#define LUX_SENSOR 0x05
#define LUX_SENSOR_PERIOD 11000
#define LUX_STRATEGIE LuxSensorStrategy()



