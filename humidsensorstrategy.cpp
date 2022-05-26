#include "humidsensorstrategy.h"

HumidSensorStrategy::HumidSensorStrategy()
{

}

int HumidSensorStrategy::getMesure()
{
    SensorsLastValue::GetInstance()->setHumidValue((float)10);
   return 0;
}

int HumidSensorStrategy::wakeUp()
{
    return 0;
}

int HumidSensorStrategy::init()
{
    return 0;
}

int HumidSensorStrategy::lowPower()
{
    return 0;
}
