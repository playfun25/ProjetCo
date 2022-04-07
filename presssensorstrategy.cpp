#include "presssensorstrategy.h"

PressSensorStrategy::PressSensorStrategy()
{

}

int PressSensorStrategy::getMesure()
{
    SensorsLastValue::GetInstance()->setpressValue(1024);
    return 1024;
}

int PressSensorStrategy::wakeUp()
{
    return 1;
}

int PressSensorStrategy::init()
{
    return 1;
}

int PressSensorStrategy::lowPower()
{
    return 1;
}
