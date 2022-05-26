#include "LoraFrame.hpp"
#include <vector>
#include <bitset>

LoraFrame::LoraFrame()
{

}

LoraFrame::~LoraFrame()
{

}

std::vector<uint8_t> LoraFrame::getFrame()
{
    std::vector<uint8_t> ret;
    for(int i = 0; i < data.size(); i+=1)
    {
        ret.push_back(dataType.at(i));

        uint8_t *array = reinterpret_cast<uint8_t*>((float*)&data.at(i));

        for(int j = 0; j < sizeof(float); j++)
        {
            ret.push_back(array[j]);
        }
    }
    return ret;
}

void LoraFrame::addData(const uint8_t& dataType, const float& data)
{
    this->dataType.push_back(dataType);
    this->data.push_back(data);
}