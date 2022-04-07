#include "sensorslastvalue.h"

SensorsLastValue* SensorsLastValue::instance = 0;

SensorsLastValue * SensorsLastValue::GetInstance()
{
    if(instance == nullptr){
        instance = new SensorsLastValue();
    }

    return instance;
}
