#include "co2sensorstrategy.h"

Co2SensorStrategy::Co2SensorStrategy()
{

}

int Co2SensorStrategy::getMesure()
{
 SensorsLastValue::GetInstance()->setCO2Value(400);
 return 0;
}

int Co2SensorStrategy::wakeUp()
{
    return 0;
}

int Co2SensorStrategy::init()
{
    return 0;
}

int Co2SensorStrategy::lowPower()
{
    return 0;
}
