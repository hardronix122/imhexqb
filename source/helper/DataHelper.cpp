#include "DataHelper.h"

uint8_t DataHelper::readByte(int &offset, std::vector<uint8_t> bytes) {
    uint8_t value = bytes[offset];
    offset++;

    return value;
}

float DataHelper::readFloat(int &offset, std::vector<uint8_t> bytes) {
    uint8_t b1 = bytes[offset];
    uint8_t b2 = bytes[offset + 1];
    uint8_t b3 = bytes[offset + 2];
    uint8_t b4 = bytes[offset + 3];

    float value;
    uint8_t value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    return value;
}

int DataHelper::readInteger(int &offset, std::vector<uint8_t> bytes) {
    uint8_t b1 = bytes[offset];
    uint8_t b2 = bytes[offset + 1];
    uint8_t b3 = bytes[offset + 2];
    uint8_t b4 = bytes[offset + 3];

    int value;
    uint8_t value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    offset += 4;

    return value;
}

int DataHelper::readReversedInteger(int &offset, std::vector<uint8_t> bytes) {
    uint8_t b1 = bytes[offset + 3];
    uint8_t b2 = bytes[offset + 2];
    uint8_t b3 = bytes[offset + 1];
    uint8_t b4 = bytes[offset];

    int value;
    uint8_t value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    offset += 4;

    return value;
}

short DataHelper::readShort(int &offset, std::vector<uint8_t> bytes) {
    if(offset + 2 > bytes.size()) {
        return -1337; // TODO: Handle this properly
    }

    uint8_t b1 = bytes[offset];
    uint8_t b2 = bytes[offset + 1];

    short value;
    uint8_t value_bytes[] = {b1, b2};
    memcpy(&value, &value_bytes, sizeof(short));

    return value;
}

std::string DataHelper::readString(int &offset, std::vector<uint8_t> bytes) {
    std::string value;

    if(offset < bytes.size() && !bytes.empty()) {
        while (bytes[offset] != 0x00) {
            value += (char) bytes[offset];

            offset++;

            if (offset >= bytes.size()) {
                break;
            }
        }
    }

    return value;
}

std::string DataHelper::readLocalString(int &offset, std::vector<uint8_t> bytes) {
    std::string value;

    if(offset > bytes.size()) {
        return value;
    }

    while(bytes[offset] != 0x00) {
        if(offset > bytes.size()) {
            break;
        }

        value += (char) readShort(offset, bytes);

        offset++;
    }

    return value;
}


Vector2f DataHelper::readPair(int &offset, const std::vector<uint8_t>& bytes) {
    float x = readFloat(offset, bytes);
    offset += 4;

    float y = readFloat(offset, bytes);
    offset += 4;

    return Vector2f(x, y);
}

Vector3f DataHelper::readVector(int &offset, const std::vector<uint8_t>& bytes) {
    float x = readFloat(offset, bytes);
    offset += 4;

    float y = readFloat(offset, bytes);
    offset += 4;

    float z = readFloat(offset, bytes);
    offset += 4;

    return Vector3f(x, y, z);
}