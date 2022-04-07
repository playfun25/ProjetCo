#include "LoraFrame.h"

LoraFrame::LoraFrame()
{

}

LoraFrame::~LoraFrame()
{

}

uint8_t* LoraFrame::getFrame() const noexcept
{
    uint8_t* ret;

    for(int i = 0; i < dataType.size(); i+=2)
    {
        ret[i] = getDataType(dataType.at(i));
        ret[i+1] = data.at(i);
    }


    return ret;
}

void LoraFrame::addData(FrameDataType dataType, uint8_t *data)
{
    this->dataType.push_back(dataType);
    this->data.push_back(*data);
}

uint8_t LoraFrame::getDataType(FrameDataType dataType) const noexcept
{
    uint8_t ret;

    switch(dataType)
    {
        case FrameDataType::hydrometrie:
            ret = HYDROMETRIE;
            break;
        case FrameDataType::temperature:
            ret = TEMPERATURE;
            break;
        case FrameDataType::luminosite:
            ret = LUMINOSITE;
            break;
        default:
            ret = 0x00;
    }

    return ret;
}