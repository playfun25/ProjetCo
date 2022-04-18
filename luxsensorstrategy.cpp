#include "luxsensorstrategy.h"

LuxSensorStrategy::LuxSensorStrategy()
{

}

int LuxSensorStrategy::getMesure()
{
    SensorsLastValue::GetInstance()->setLumiValue(10000);
    return 0;
}


int LuxSensorStrategy::wakeUp()
{
    return 1;
}

int LuxSensorStrategy::init()
{
    return 1;
}

int LuxSensorStrategy::lowPower()
{
    return 1;
}
