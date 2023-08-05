#ifndef CSTRUCTELEMENTS_H
#define CSTRUCTELEMENTS_H

#include <cstdint>
#include <string>
#include <vector>
#include "../../type/Vectors.h"

struct CStructBase {
    uint8_t type;
    uint32_t name;
};

struct CStructInteger : CStructBase {
    int32_t value;
};

struct CStructByte : CStructBase {
    int8_t value;
};

struct CStructUnsignedByte : CStructBase {
    uint8_t value;
};

struct CStructShort : CStructBase {
    int16_t value;
};

struct CStructUnsignedShort : CStructBase {
    uint16_t value;
};

struct CStructFloat : CStructBase {
    float value;
};

struct CStructName : CStructBase {
    uint32_t value;
};

struct CStructString : CStructBase {
    std::string value;
};

struct CStructLocalString : CStructBase {
    std::string value;
};

struct CStructPair : CStructBase {
    Vector2f value;
};

struct CStructVector : CStructBase {
    Vector3f value;
};

struct CStructStructure : CStructBase {
    std::vector<CStructBase> value;
};

struct CStructArray : CStructBase {
    std::vector<CStructBase> value;
};

#endif //CSTRUCTELEMENTS_H
