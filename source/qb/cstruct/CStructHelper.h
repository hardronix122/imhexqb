#ifndef CSTRUCT_HELPER_H
#define CSTRUCT_HELPER_H

#include "../../helper/DataHelper.h"
#include "../../type/Vectors.h"
#include "CStructElements.h"
#include "CStructCComponent.h"

class CStructHelper {
public:
    static int readInteger(int &index, CStructCComponent &component);

    static int8_t readByte(int &index, CStructCComponent &component);

    static uint8_t readUnsignedByte(int &index, CStructCComponent &component);

    static int16_t readShort(int &index, CStructCComponent &component);

    static uint16_t readUnsignedShort(int &index, CStructCComponent &component);

    static uint32_t readName(int &index, CStructCComponent &component);

    static std::string readString(int &index, CStructCComponent &component);

    static Vector2f readPair(int &index, CStructCComponent &component);

    static Vector3f readVector(int &index, CStructCComponent &component);

    static CStructStructure deserializeStructure(int &offset, const std::vector<uint8_t> &bytes);

    static CStructArray deserializeArray(int &offset, const std::vector<uint8_t> &bytes);
};


#endif //CSTRUCT_HELPER_H
