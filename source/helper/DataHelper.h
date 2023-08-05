#ifndef DATA_HELPER_H
#define DATA_HELPER_H

#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
#include "../type/Vectors.h"

class DataHelper {
public:
    static uint8_t readByte(int &offset, std::vector<uint8_t> bytes);

    static float readFloat(int &offset, std::vector<uint8_t> bytes);

    static int readInteger(int &offset, std::vector<uint8_t> bytes);

    static int readReversedInteger(int &offset, std::vector<uint8_t> bytes);

    static int16_t readShort(int &offset, std::vector<uint8_t> bytes);

    static std::string readString(int &offset, std::vector<uint8_t> bytes);

    static std::string readLocalString(int &offset, std::vector<uint8_t> bytes);

    static Vector2f readPair(int &offset, const std::vector<uint8_t> &bytes);

    static Vector3f readVector(int &offset, const std::vector<uint8_t>& bytes);
};


#endif //DATA_HELPER_H
