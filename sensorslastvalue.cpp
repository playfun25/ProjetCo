#include "sensorslastvalue.h"

// On definit l'instance
SensorsLastValue* SensorsLastValue::instance = nullptr;

SensorsLastValue * SensorsLastValue::GetInstance()
{
    if(instance == nullptr)
    {
        instance = new SensorsLastValue();
    }

    return instance;
}
