#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

constexpr uint8_t TEMPERATURE = 0x01;
constexpr uint8_t HYDROMETRIE = 0x02;
constexpr uint8_t LUMINOSITE = 0x03;

enum class FrameDataType : uint8_t {
    temperature = TEMPERATURE,
    hydrometrie = HYDROMETRIE,
    luminosite = LUMINOSITE
};

class LoraFrame
{
    ///****************************************************************************************************///
    /// ATTRIBUTS
    ///****************************************************************************************************///
    private:
        std::vector<FrameDataType> dataType;
        std::vector<uint8_t> data;

    ///****************************************************************************************************///
    /// CONSTRUCTEUR ET DESTRUCTEUR
    ///****************************************************************************************************///
    public:
        LoraFrame();
        virtual ~LoraFrame();

    ///****************************************************************************************************///
    /// METHODES
    ///****************************************************************************************************///
    public:
        uint8_t *getFrame() const noexcept;
        void addData(FrameDataType dataType, uint8_t *data);
        uint8_t getDataType(FrameDataType dataType) const noexcept;
};