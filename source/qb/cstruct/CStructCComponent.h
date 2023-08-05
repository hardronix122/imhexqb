#ifndef CSTRUCT_C_COMPONENT_H
#define CSTRUCT_C_COMPONENT_H

#include <cstdint>
#include <vector>
#include "CStructType.h"

class CStructCComponent {
private:
    uint32_t name;
    CStructType type;
    std::vector<uint8_t> rawData;
public:
    void setName(uint32_t componentName);
    void setType(CStructType componentType);
    void setData(std::vector<uint8_t> componentData);

    uint32_t getName() const;
    CStructType getType();
    std::vector<uint8_t> getRawData();
};


#endif //CSTRUCT_C_COMPONENT_H
