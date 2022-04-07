#pragma once

class SensorsLastValue;

class SensorsLastValue
{

private:
    SensorsLastValue() {};
    float temp;
    float humid;
    float CO2;
    float eCO2;
    float COV;
    float Lumi;
    float UV;
    float press;
    static SensorsLastValue * instance;


public:


      float getTempValue(){ return temp;};
      float getHumidValue(){ return humid;};
      float getCO2Value(){ return CO2;};
      float geteCO2Value(){ return eCO2;};
      float getCOVValue(){ return COV;};
      float getLumiValue(){ return Lumi;};
      float getUVValue(){ return UV;};
      float getpressValue(){ return press;};



      void setTempValue(float value){ temp = value ;};
      void setHumidValue(float value){  humid = value;};
      void setCO2Value(float value){  CO2 = value;};
      void seteCO2Value(float value){  eCO2 = value;};
      void setCOVValue(float value){  COV = value;};
      void setLumiValue(float value){  Lumi = value;};
      void setUVValue(float value){  UV = value;};
      void setpressValue(float value){  press = value;};

      static SensorsLastValue* GetInstance();


};

