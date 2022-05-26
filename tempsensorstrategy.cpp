#include "tempsensorstrategy.h"


int tempSensorStrategy::getMesure()
{
    SensorsLastValue::GetInstance()->setTempValue((float)10);
    return 0;
}


int tempSensorStrategy::wakeUp()
{
    return 0;
}

int tempSensorStrategy::init()
{
    return 0;
}

int tempSensorStrategy::lowPower()
{
    return 0;
}
