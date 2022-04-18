#include "tempsensorstrategy.h"


int tempSensorStrategy::getMesure()
{
    SensorsLastValue::GetInstance()->setTempValue(1);
    return 0;
}


int tempSensorStrategy::wakeUp()
{
    return 1;
}

int tempSensorStrategy::init()
{
    return 1;
}

int tempSensorStrategy::lowPower()
{
    return 1;
}
