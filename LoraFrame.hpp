#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>



/**
 * @brief Represente une trame LoRa
 * 
 */
class LoraFrame
{
    ///****************************************************************************************************///
    /// ATTRIBUTS
    ///****************************************************************************************************///
    private:
        /**
         * @brief Vecteur des types de donnes
         * 
         */
        std::vector<uint8_t> dataType;

        /**
         * @brief Vecteur des valeurs
         * 
         */
        std::vector<float> data;

    ///****************************************************************************************************///
    /// CONSTRUCTEUR ET DESTRUCTEUR
    ///****************************************************************************************************///
    public:
        /**
         * @brief Construit une nouvelle trame LoRa
         * 
         */
        LoraFrame();

        /**
         * @brief Detruit la trame LoRa
         * 
         */
        virtual ~LoraFrame();

    ///****************************************************************************************************///
    /// METHODES
    ///****************************************************************************************************///
    public:
        /**
         * @brief Recuere la trame sous le format dataType|data|dataType|data 
         * 
         * @return std::vector<uint8_t> retourne la trame dans un vecteur uint8_t
         */
        std::vector<uint8_t> getFrame();

        /**
         * @brief Recuere la trame sous le format dataType|data|dataType|data en chaine de caractere
         * 
         * @return std::string retourne la trame formate en chaine de caractere
         */
        std::string getFrame_str();

        /**
         * @brief Ajout une nouvelle donnees et son type aux vecteurs
         * 
         * @param dataType le type de la nouvelle donnee
         * @param data la nouvelle donnee
         */
        void addData(const uint8_t& dataType, const float& data);
};