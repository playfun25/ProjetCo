#include "sensor.h"



Sensor::Sensor(int _period, uint8_t _type, SensorStrategy * _strategy):period(_period),periodLeft(_period),type(_type)
{
    this->strategy = _strategy;
}

void Sensor::resetPeriod()
{
    this->periodLeft = this->period;
}

int Sensor::getPeriodLeft() const
{
    return periodLeft;
}

int Sensor::getMesure()
{
  return this->strategy->getMesure();
}

int Sensor::getType() const
{
    return type;
}

int Sensor::decrementPeriod(int value)
{
 this->periodLeft -= value;
 return this->periodLeft;
}
