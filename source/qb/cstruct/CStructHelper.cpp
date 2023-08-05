#include "CStructHelper.h"

int CStructHelper::readInteger(int &index, CStructCComponent &component) {
    return DataHelper::readInteger(index, component.getRawData());
}

int8_t CStructHelper::readByte(int &index, CStructCComponent &component) {
    return (int8_t) DataHelper::readByte(index, component.getRawData());
}

uint8_t CStructHelper::readUnsignedByte(int &index, CStructCComponent &component) {
    return DataHelper::readByte(index, component.getRawData());
}

int16_t CStructHelper::readShort(int &index, CStructCComponent &component) {
    return DataHelper::readShort(index, component.getRawData());
}

uint16_t CStructHelper::readUnsignedShort(int &index, CStructCComponent &component) {
    return (uint16_t) DataHelper::readShort(index, component.getRawData());
}

uint32_t CStructHelper::readName(int &index, CStructCComponent &component) {
    return (uint32_t) DataHelper::readInteger(index, component.getRawData());
}

std::string CStructHelper::readString(int &index, CStructCComponent &component) {
    return DataHelper::readString(index, component.getRawData());
}

Vector2f CStructHelper::readPair(int &index, CStructCComponent &component) {
    return DataHelper::readPair(index, component.getRawData());
}

Vector3f CStructHelper::readVector(int &index, CStructCComponent &component) {
    return DataHelper::readVector(index, component.getRawData());
}

const char* enums[] {
        "NONE",
        "INTEGER",
        "FLOAT",
        "STRING",
        "LOCAL_STRING",
        "PAIR",
        "VECTOR",
        "SCRIPT",
        "C_FUNCTION",
        "MEMBER_FUNCTION",
        "STRUCTURE",
        "STRUCTURE_POINTER",
        "ARRAY",
        "NAME",
        "BYTE",
        "SHORT",
        "UNSIGNED_BYTE",
        "UNSIGNED_SHORT",
        "ZERO_INTEGER",
        "ZERO_FLOAT",
        "STRUCTURE_REFERENCE",
        "ARRAY_REFERENCE",
        "STATIC_FAST_STRUCTURE",
        "DYNAMIC_FAST_STRUCTURE",
        "FLAG",
        "TEMPLATE"
};

CStructStructure CStructHelper::deserializeStructure(int &offset, const std::vector<uint8_t> &bytes) {
    CStructStructure structure = CStructStructure();

    if(offset > bytes.size()) {
        return structure;
    }

    while (offset < bytes.size()) {
        if(bytes.empty()) {
            break;
        }

        uint8_t type = DataHelper::readByte(offset, bytes);

        if (type == NONE) {
            break;
        }

        if(offset + 4 > bytes.size()) {
            break;
        }

        uint32_t name = DataHelper::readInteger(offset, bytes);

        switch (type) {
            case INTEGER:
            case ZERO_INTEGER: {
                if(offset + 4 > bytes.size()) {
                    return structure;
                }

                int value = DataHelper::readInteger(offset, bytes);

                CStructInteger integerStruct = CStructInteger();
                integerStruct.name = name;

                if(value < 1) {
                    integerStruct.type = ZERO_INTEGER;
                } else {
                    integerStruct.type = INTEGER;
                }

                integerStruct.value = value;

                structure.value.push_back(integerStruct);
                break;
            }
            case BYTE: {
                if(offset + 1 > bytes.size()) {
                    return structure;
                }

                int8_t value = (int8_t) DataHelper::readByte(offset, bytes);

                CStructByte byteStruct = CStructByte();
                byteStruct.name = name;
                byteStruct.type = BYTE;
                byteStruct.value = value;

                structure.value.push_back(byteStruct);
                break;
            }
            case UNSIGNED_BYTE: {
                if(offset + 1 > bytes.size()) {
                    return structure;
                }

                uint8_t value = DataHelper::readByte(offset, bytes);

                CStructUnsignedByte unsignedByteStruct = CStructUnsignedByte();
                unsignedByteStruct.name = name;
                unsignedByteStruct.type = UNSIGNED_BYTE;
                unsignedByteStruct.value = value;

                structure.value.push_back(unsignedByteStruct);
                break;
            }
            case SHORT: {
                if(offset + 2 > bytes.size()) {
                    return structure;
                }

                int16_t value = DataHelper::readShort(offset, bytes);

                offset += 2;

                CStructShort shortStruct = CStructShort();
                shortStruct.name = name;
                shortStruct.type = SHORT;
                shortStruct.value = value;

                structure.value.push_back(shortStruct);
                break;
            }
            case UNSIGNED_SHORT: {
                if(offset + 2 > bytes.size()) {
                    return structure;
                }

                uint16_t value = (uint16_t) DataHelper::readShort(offset, bytes);

                offset += 2;

                CStructUnsignedShort unsignedShortStruct = CStructUnsignedShort();
                unsignedShortStruct.name = name;
                unsignedShortStruct.type = UNSIGNED_SHORT;
                unsignedShortStruct.value = value;

                structure.value.push_back(unsignedShortStruct);
                break;
            }
            case FLOAT:
            case ZERO_FLOAT: {
                if(offset + 4 > bytes.size()) {
                    return structure;
                }

                float value = DataHelper::readFloat(offset, bytes);

                offset += 4;

                CStructFloat floatStruct = CStructFloat();
                floatStruct.name = name;

                if(std::abs(value) < 0.0001) {
                    floatStruct.type = ZERO_FLOAT;
                } else {
                    floatStruct.type = FLOAT;
                }

                floatStruct.value = value;

                structure.value.push_back(floatStruct);
                break;
            }
            case STRING: {
                std::string value = DataHelper::readString(offset, bytes);

                CStructString stringStruct = CStructString();
                stringStruct.name = name;
                stringStruct.type = STRING;
                stringStruct.value = value;

                structure.value.push_back(stringStruct);
                break;
            }
            case LOCAL_STRING: {
                std::string value = DataHelper::readLocalString(offset, bytes);

                CStructLocalString localStringStruct = CStructLocalString();
                localStringStruct.name = name;
                localStringStruct.type = LOCAL_STRING;
                localStringStruct.value = value;

                structure.value.push_back(localStringStruct);
                break;
            }
            case PAIR: {
                if(offset + 8 > bytes.size()) {
                    return structure;
                }

                Vector2f value = DataHelper::readPair(offset, bytes);

                CStructPair pairStruct = CStructPair();
                pairStruct.name = name;
                pairStruct.type = PAIR;
                pairStruct.value = value;

                structure.value.push_back(pairStruct);
                break;
            }
            case VECTOR: {
                if(offset + 12 > bytes.size()) {
                    return structure;
                }

                Vector3f value = DataHelper::readVector(offset, bytes);

                CStructVector vectorStruct = CStructVector();
                vectorStruct.name = name;
                vectorStruct.type = VECTOR;
                vectorStruct.value = value;

                structure.value.push_back(vectorStruct);
                break;
            }
            case STRUCTURE: {
                CStructStructure cstruct = deserializeStructure(offset, bytes);
                cstruct.type = STRUCTURE;

                structure.value.push_back(cstruct);
                break;
            }
            case ARRAY: {
                CStructArray array = deserializeArray(offset, bytes);
                array.type = ARRAY;

                structure.value.push_back(array);
                break;
            }
            default:
                break;
        }
    }


    return structure;
}

CStructArray CStructHelper::deserializeArray(int &offset, const std::vector<uint8_t> &bytes) {
    CStructArray array = CStructArray();

    if(bytes.size() < 3) {
        return array;
    }

    uint8_t type = DataHelper::readByte(offset, bytes);
    uint16_t size = DataHelper::readShort(offset, bytes);

    if(type < 25) {
        printf("Type: %s\n", enums[type]);
    } else {
        printf("Type: %d\n", type);
    }

    if(offset > bytes.size()) {
        return array;
    }

    switch (type) {
        case INTEGER:
        case ZERO_INTEGER: {
            if(offset + 4 > bytes.size()) {
                return array;
            }

            for (int i = 0; i < size; i++) {
                CStructInteger cstruct = CStructInteger();
                cstruct.name = 0;
                cstruct.type = INTEGER;
                cstruct.value = DataHelper::readInteger(offset, bytes);

                array.value.push_back(cstruct);
            }
            break;
        }
        case BYTE: {
            if(offset + 1 > bytes.size()) {
                return array;
            }

            for (int i = 0; i < size; i++) {
                CStructByte cstruct = CStructByte();
                cstruct.name = 0;
                cstruct.type = BYTE;
                cstruct.value = (int8_t) DataHelper::readByte(offset, bytes);

                array.value.push_back(cstruct);
            }
            break;
        }
        case UNSIGNED_BYTE: {
            if(offset + 1 > bytes.size()) {
                return array;
            }

            for (int i = 0; i < size; i++) {
                CStructUnsignedByte cstruct = CStructUnsignedByte();
                cstruct.name = 0;
                cstruct.type = UNSIGNED_BYTE;
                cstruct.value = DataHelper::readByte(offset, bytes);

                array.value.push_back(cstruct);
            }
            break;
        }
        case SHORT: {
            if(offset + 2 > bytes.size()) {
                return array;
            }

            for(int i = 0; i < size; i++) {
                CStructShort cstruct = CStructShort();
                cstruct.name = 0;
                cstruct.type = SHORT;
                cstruct.value = DataHelper::readShort(offset, bytes);

                offset += 2;

                array.value.push_back(cstruct);
            }
            break;
        }
        case UNSIGNED_SHORT: {
            if(offset + 2 > bytes.size()) {
                return array;
            }

            for(int i = 0; i < size; i++) {
                CStructUnsignedShort cstruct = CStructUnsignedShort();
                cstruct.name = 0;
                cstruct.type = UNSIGNED_SHORT;
                cstruct.value = (uint16_t) DataHelper::readShort(offset, bytes);

                offset += 2;

                array.value.push_back(cstruct);
            }
            break;
        }
        case FLOAT:
        case ZERO_FLOAT: {
            if(offset + 4 > bytes.size()) {
                return array;
            }

            for(int i = 0; i < size; i++) {
                CStructFloat cstruct = CStructFloat();
                cstruct.name = 0;
                cstruct.value = DataHelper::readFloat(offset, bytes);

                offset += 4;

                if(std::abs(cstruct.value) < 0.0001) {
                    cstruct.value = ZERO_FLOAT;
                } else {
                    cstruct.value = FLOAT;
                }

                array.value.push_back(cstruct);
            }

            break;
        }
        case NAME: {
            if(offset + 4 > bytes.size()) {
                return array;
            }

            for(int i = 0; i < size; i++) {
                CStructName cstruct = CStructName();
                cstruct.name = 0;
                cstruct.type = NAME;
                cstruct.value = DataHelper::readInteger(offset, bytes);

                array.value.push_back(cstruct);
            }

            break;
        }
        case STRING: {
            for(int i = 0; i < size; i++) {
                CStructString cstruct = CStructString();
                cstruct.name = 0;
                cstruct.type = STRING;
                cstruct.value = DataHelper::readString(offset, bytes);

                array.value.push_back(cstruct);
            }

            break;
        }
        case LOCAL_STRING: {
            for(int i = 0; i < size; i++) {
                CStructLocalString cstruct = CStructLocalString();
                cstruct.name = 0;
                cstruct.type = LOCAL_STRING;
                cstruct.value = DataHelper::readLocalString(offset, bytes);

                array.value.push_back(cstruct);
            }

            break;
        }
        case PAIR: {
            if(offset + 8 > bytes.size()) {
                return array;
            }

            for (int i = 0; i < size; i++) {
                CStructPair cstruct = CStructPair();
                cstruct.name = 0;
                cstruct.type = PAIR;
                cstruct.value = DataHelper::readPair(offset, bytes);

                array.value.push_back(cstruct);
            }

            break;
        }
        case VECTOR: {
            if(offset + 12 > bytes.size()) {
                return array;
            }

            for(int i = 0; i < size; i++) {
                CStructVector cstruct = CStructVector();
                cstruct.name = 0;
                cstruct.type = VECTOR;
                cstruct.value = DataHelper::readVector(offset, bytes);

                array.value.push_back(cstruct);
            }

            break;
        }
        case STRUCTURE: {
            for(int i = 0; i < size; i++) {
                CStructStructure cstruct = deserializeStructure(offset, bytes);
                cstruct.type = STRUCTURE;

                array.value.push_back(cstruct);
            }

            break;
        }
        case ARRAY: {
            for(int i = 0; i < size; i++) {
                CStructArray cstruct = deserializeArray(offset, bytes);
                cstruct.type = ARRAY;

                array.value.push_back(cstruct);
            }

            break;
        }
        default:
            printf("UNKNOWN\n");
            break; // TODO: Handle this properly later on
    }

    return array;
}

