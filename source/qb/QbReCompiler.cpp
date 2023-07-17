#include <cstring>
#include "QbReCompiler.h"

std::string QbReCompiler::decompile(std::vector<u8> bytes, std::map<int32_t, std::string> &symbols, bool greedySymbolCapture) {
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

                    if(symbols.contains(checksum)) {
                        code += "$" + symbols[checksum] + "$";
                    } else {
                        code += "$[" + std::to_string(checksum) + "]$";
                    }

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

                    while (i < bytes.size()) {
                        if (bytes[i] == 0x00) {
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
            case 0x20: // Begin
                code += "while";
                break;
            case 0x21: // Repeat
                code += "loop_to";
                break;
            case 0x22: // Break
                code += "break";
                break;
            case 0x23: // Function
                code += "function ";
                break;
            case 0x24: // End Function
                code += "endfunction";
                break;
            case 0x25: // If
                code += "if ";
                break;
            case 0x26: // Else
                code += "else";
                break;
            case 0x27: // Elseif
                code += "elseif";
                break;
            case 0x28: // Endif
                code += "endif";
                break;
            case 0x29: // Return
                code += "return";
                break;
            case 0x2A: // Undefined
                break;
            case 0x2B: // Symbol entry
                if (i + 4 <= bytes.size()) {
                    int checksum = readInt(i, bytes);
                    i += 4;

                    std::string name;

                    while (i < bytes.size()) {
                        if (bytes[i] == 0x00) {
                            i++;
                            break;
                        }

                        name += static_cast<char>(bytes[i]);

                        i++;
                    }

                    //code += "\n#/ Symbol entry: " + std::to_string(checksum) + " = " + value + "\n";

                    if(!symbols.contains(checksum) && greedySymbolCapture) {
                        symbols.insert(std::pair<int32_t, std::string>(checksum, name));
                    }
                }
                break;
            case 0x2C: // AllArgs
                code += " isNull";
                break;
            case 0x2D: // Argument stack or Global
                code += "%GLOBAL%";
                break;
            case 0x2E: // TODO: Implement jump
            case 0x2F: // TODO: Implement random
            case 0x30: // TODO: Implement random range
            case 0x31: // TODO: Implement at
                break;
            case 0x32: // "Or" condition
            case 0x3B: // "Or" condition
                code += " OR ";
                break;
            case 0x33: // "And" condition
            case 0x3A: // "And" condition
                code += " AND ";
                break;
            case 0x34: // "Or"? condition
                code += " OR ? ";
                break;
            case 0x35: // Shift left
                code += " << ";
                break;
            case 0x36: // Shift right
                code += " >> ";
                break;
            case 0x37: // TODO: Implement random 2
            case 0x38: // TODO: Implement random range 2
                break;
            case 0x39: // "Not" condition
                code += "NOT ";
                break;
            case 0x3C: // Switch expression
                code += "switch ";
                break;
            case 0x3D: // Endswitch expression
                code += "endswitch";
                break;
            case 0x3E: // Case expression
                code += "case ";
                break;
            case 0x3F: // Default case expression
                code += "default";
                break;
            case 0x40: // TODO: Implement random. Won't merge it with previous, as it might be different
            case 0x41: // TODO: Implement random for this one too
                break;
            case 0x42: // Colon
                code += ".";
                break;
            case 0x43: // TODO: Never used
            case 0x44: // TODO: Never used
            case 0x45: // TODO: Unknown
            case 0x46: // TODO: Unknown
                break;
            case 0x47: // If expression with short offset
                if (i + 2 < bytes.size()) {
                    short offset = readShort(i, bytes); // TODO: Figure out what that offset is used for
                    i += 2;

                    code += "if[" + std::to_string(offset) + "] ";
                }

                break;
            case 0x48: // Else expression with short offset
                if (i + 2 < bytes.size()) {
                    short offset = readShort(i, bytes); // TODO: Figure out what that offset is used for
                    i += 2;

                    code += "else[" + std::to_string(offset) + "] ";
                }

                break;
            case 0x49: // Short break
                if (i + 2 < bytes.size()) {
                    short offset = readShort(i, bytes); // TODO: Figure out what that offset is used for
                    i += 2;

                    code += "shortbreak[" + std::to_string(offset) + "] ";
                }

                break;
            default:
                code += " <[!Unknown Instruction " + std::to_string(byte) + "!]> ";
                break;
        }
    }

    return code;
}

std::vector<u8> QbReCompiler::compile(std::string source) {
    std::vector<u8> bytes = std::vector<u8>();

    return bytes;
}

float QbReCompiler::readFloat(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset];
    u8 b2 = bytes[offset + 1];
    u8 b3 = bytes[offset + 2];
    u8 b4 = bytes[offset + 3];

    float value;
    u8 value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    return value;
}

int QbReCompiler::readInt(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset];
    u8 b2 = bytes[offset + 1];
    u8 b3 = bytes[offset + 2];
    u8 b4 = bytes[offset + 3];

    int value;
    u8 value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    return value;
}

short QbReCompiler::readShort(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset];
    u8 b2 = bytes[offset + 1];

    short value;
    u8 value_bytes[] = {b1, b2};
    memcpy(&value, &value_bytes, sizeof(short));
    return value;
}
