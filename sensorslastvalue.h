#pragma once



/**
 * @brief Singleton, stock toutes les valeurs des capteurs
 * 
 */
class SensorsLastValue
{

private:
    /**
     * @brief Construit un nouvelle objet
     * 
     */
    SensorsLastValue() {};

    /**
     * @brief Temperature
     * 
     */
    float temp;

    /**
     * @brief Humidite
     * 
     */
    float humid;

    /**
     * @brief CO2
     * 
     */
    float CO2;

    /**
     * @brief eCO2
     * 
     */
    float eCO2;

    /**
     * @brief COV
     * 
     */
    float COV;

    /**
     * @brief Luminosite
     * 
     */
    float Lumi;

    /**
     * @brief UV
     * 
     */
    float UV;

    /**
     * @brief Pression atmosphérique
     * 
     */
    float press;

    /**
     * @brief Unique instance du singleton
     * 
     */
    static SensorsLastValue * instance;


public:

    /**
     * @brief Recupere la valeur de la derniere mesure de a temperature
     * 
     * @return float retourne la temperature
     */
    float getTempValue(){ return temp;};

    /**
     * @brief Recupere la valeur de la derniere mesure d'humidite
     * 
     * @return float retourne la temperature
     */
    float getHumidValue(){ return humid;};

    /**
     * @brief Recupere la valeur de la derniere mesure de CO2
     * 
     * @return float retourne le CO2
     */
    float getCO2Value(){ return CO2;};

    /**
     * @brief Recupere la valeur de la derniere mesure de eCO2
     * 
     * @return float retourne le eCO2
     */
    float geteCO2Value(){ return eCO2;};

    /**
     * @brief Recupere la valeur de la derniere mesure de COV
     * 
     * @return float retourne le COV
     */
    float getCOVValue(){ return COV;};

    /**
     * @brief Recupere la valeur de la derniere mesure de luminosite
     * 
     * @return float retourne la luminosite
     */
    float getLumiValue(){ return Lumi;};

    /**
     * @brief Recupere la valeur de la derniere mesure d'UV
     * 
     * @return float retourne l'UV
     */
    float getUVValue(){ return UV;};

    /**
     * @brief Recupere la valeur de la derniere mesure de pression atmospherique
     * 
     * @return float retourne la pression atmospherique
     */
    float getpressValue(){ return press;};


    /**
     * @brief Fixe la valeur de la temperature
     * 
     * @param value temperature
     */
    void setTempValue(float value){ temp = value ;};

    /**
     * @brief Fixe la valeur de l'humidite
     * 
     * @param value humidite
     */
    void setHumidValue(float value){  humid = value;};

    /**
     * @brief Fixe la valeur du CO2
     * 
     * @param value CO2
     */
    void setCO2Value(float value){  CO2 = value;};

    /**
     * @brief Fixe la valeur de l'eCO2
     * 
     * @param value temperature
     */
    void seteCO2Value(float value){  eCO2 = value;};

    /**
     * @brief Fixe la valeur du COV
     * 
     * @param value COV
     */
    void setCOVValue(float value){  COV = value;};

    /**
     * @brief Fixe la valeur de la luminosite
     * 
     * @param value luminosite
     */
    void setLumiValue(float value){  Lumi = value;};

    /**
     * @brief Fixe la valeur de l'UV
     * 
     * @param value UV
     */
    void setUVValue(float value){  UV = value;};

    /**
     * @brief Fixe la valeur de la pression atmospherique
     * 
     * @param value atmospherique
     */
    void setpressValue(float value){  press = value;};


    /**
     * @brief Recupere l'unique instance
     * 
     * @return SensorsLastValue* retourne l'unique instance de l'objet
     */
    static SensorsLastValue* GetInstance();

};

