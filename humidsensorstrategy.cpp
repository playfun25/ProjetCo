#include "humidsensorstrategy.h"

HumidSensorStrategy::HumidSensorStrategy()
{

}

int HumidSensorStrategy::getMesure()
{
    SensorsLastValue::GetInstance()->setHumidValue(25);
   return 0;
}

int HumidSensorStrategy::wakeUp()
{
    return 1;
}

int HumidSensorStrategy::init()
{
    return 1;
}

int HumidSensorStrategy::lowPower()
{
    return 1;
}
