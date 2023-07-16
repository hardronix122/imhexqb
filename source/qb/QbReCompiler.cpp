#include <cstring>
#include "QbReCompiler.h"

std::string QbReCompiler::decompile(std::vector<u8> bytes) {
    std::string code;

    if (bytes[0] != 0x01) {
        code += "#/ Owops! This chunk doesn't start with a new instruction!\n#/ Not big of a deal if you know what you're actually doing, just wanna let ya know :3\n";
    }

    size_t i = 0;

    while (i < bytes.size()) {
        u8 byte = bytes[i];

        i++;

        switch (byte) {
            case 0x01: // New Instruction
                code += "\n:i ";
                break;
            case 0x02: // End Of Line (unused)
                i += 4; // Skipping line number
                break;
            case 0x03: // Struct
                code += ":s{";
                break;
            case 0x04: // End Struct
                code += ":s}";
                break;
            case 0x05: // Array
                code += ":a{";
                break;
            case 0x06: // End Array
                code += ":a}";
                break;
            case 0x07: // Equals
                code += " = ";
                break;
            case 0x08: // Dot
                code += ".";
                break;
            case 0x09: // Comma
                code += ",";
                break;
            case 0xA: // Minus
                code += " - ";
                break;
            case 0xB: // Plus
                code += " + ";
                break;
            case 0xC: // Multiply
                code += " * ";
                break;
            case 0xD: // Divide
                code += " / ";
                break;
            case 0xE: // Nesting increase
            case 0xF: // Nesting decrease
                break;
            case 0x10: // Never? Whatever..
                code += "<![This instruction shouldn't be here!]>";
                break;
            case 0x11: // Equals
                code += " == ";
                break;
            case 0x12: // Less than
                code += " < ";
                break;
            case 0x13: // Less than or equals
                code += " <= ";
                break;
            case 0x14: // More than
                code += " > ";
                break;
            case 0x15: // More than or equals
                code += " >= ";
                break;
            case 0x16: // Checksum
                if (i + 4 <= bytes.size()) {
                    int checksum = readInt(i, bytes);

                    code += "$[" + std::to_string(checksum) + "]$";

                    i += 4;
                }

                break;
            case 0x17: // Long integer
                if (i + 4 <= bytes.size()) {
                    int value = readInt(i, bytes);

                    code += "%i(" + std::to_string(value) + ".00000000)";

                    i += 4;
                }
                break;
            case 0x18: // TODO: handle Hex Integer
            case 0x19: // TODO: Implement enum
                break;
            case 0x1A: // Single float
                if (i + 4 <= bytes.size()) {
                    float value = readFloat(i, bytes);

                    code += "%f(" + std::to_string(value) + ")";

                    i += 4;
                }

                break;
            case 0x1B: // String
            case 0x1C: // String
                if (i + 4 <= bytes.size()) {
                    int size = readInt(i, bytes);

                    code += "%s(" + std::to_string(size) + ", ";

                    i += 4;

                    std::string value;

                    while(i < bytes.size()) {
                        if(bytes[i] == 0x00) {
                            i++;
                            break;
                        }

                        value += static_cast<char>(bytes[i]);

                        i++;
                    }

                    code += "\"" + value + "\")";
                }
                break;
            case 0x1D: // Array
                code += ":a{";
                break;
            case 0x1E: // Vector
                if (i + 12 <= bytes.size()) {
                    float x = readFloat(i, bytes);
                    i += 4;
                    float y = readFloat(i, bytes);
                    i += 4;
                    float z = readFloat(i, bytes);
                    i += 4;

                    code += "%vec3(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
                }

                break;
            case 0x1F: // Pair
                if (i + 8 <= bytes.size()) {
                    float x = readFloat(i, bytes);
                    i += 4;
                    float y = readFloat(i, bytes);
                    i += 4;

                    code += "%vec2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
                }

                break;
            default:
                code += " <[!Unknown Instruction " + std::to_string(byte) + "!]> ";
                break;
        }
    }

    return code;
}

float QbReCompiler::readFloat(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset];
    u8 b2 = bytes[offset + 1];
    u8 b3 = bytes[offset + 2];
    u8 b4 = bytes[offset + 3];

    float value;
    u_char value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    return value;
}

int QbReCompiler::readInt(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset];
    u8 b2 = bytes[offset + 1];
    u8 b3 = bytes[offset + 2];
    u8 b4 = bytes[offset + 3];

    int value;
    u_char value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    return value;
}
