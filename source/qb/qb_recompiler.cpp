#include "qb_recompiler.h"

std::string
qb_recompiler::decompile(std::vector<u8> bytes, std::map<int32_t, std::string> &symbols, bool greedySymbolCapture) {
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
                    int checksum = readInvInt(i, bytes);

                    if (symbols.contains(checksum)) {
                        code += "$" + symbols[checksum] + "$";
                    } else {
                        std::stringstream ss;
                        ss << std::setfill('0') << std::setw(sizeof(int) * 2) << std::hex << checksum;
                        code += "$[" + ss.str() + "]$";
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
                    int checksum = readInvInt(i, bytes);
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

                    if (!symbols.contains(checksum) && greedySymbolCapture) {
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

std::vector<u8> qb_recompiler::compile(std::string &source) {
    std::vector<u8> bytes = std::vector<u8>();

    std::istringstream sourceStream(source);
    std::string line;

    while (std::getline(sourceStream, line)) {
        int index = 0;

        while (line.size() > index) {
            if (line.size() >= 2) {

                /*
                * Operator name: Comment
                * Operands: None
                * Format: #/
                * Algorithm:
                * Skip the line
                */

                if (line[index] == '#' && line[index + 1] == '/') {
                    break;
                }

                /*
                * Operator name: New Instruction [0x01]
                * Operands: None
                * Format: :i
                *
                * Algorithm:
                * Insert if the next sequence is ":i"
                */

                if (line[index] == ':' && line[index + 1] == 'i') {
                    bytes.push_back(0x01);
                }

                /*
                * Operator name: Equals 2 [0x11]
                * Operands: None
                * Format: ==
                *
                * Algorithm:
                * Insert if the next sequence is "=="
                */

                if (line[index] == '=' && line[index + 1] == '=') {
                    bytes.push_back(0x11);
                }

                /*
                * Operator name: Less Than Or Equals [0x13]
                * Operands: None
                * Format: <=
                *
                * Algorithm:
                * Insert if the next sequence is "<="
                */

                if (line[index] == '<' && line[index + 1] == '=') {
                    bytes.push_back(0x13);
                }

                /*
                * Operator name: More Than Or Equals [0x15]
                * Operands: None
                * Format: >=
                *
                * Algorithm:
                * Insert if the next sequence is ">="
                */

                if (line[index] == '>' && line[index + 1] == '=') {
                    bytes.push_back(0x15);
                }

                if(line.size() >= 3) {
                    /*
                    * Operator name: Struct [0x03] + Struct End [0x04}
                    * Operands: None
                    * Format: :s{ / :s}
                    *
                    * Algorithm:
                    * Insert struct begin if the next sequence is :s{, insert struct end if the next sequence is :s}
                    */

                    if(line[index] == ':' && line[index + 1] == 's') {
                        if(line[index + 2] == '{') {
                            bytes.push_back(0x03);
                        } else if(line[index + 2] == '}') {
                            bytes.push_back(0x04);
                        }
                    }

                    /*
                    * Operator name: Array [0x05] + Array End [0x06}
                    * Operands: None
                    * Format: :a{ / :a}
                    *
                    * Algorithm:
                    * Insert array begin if the next sequence is :a{, insert struct end if the next sequence is :a}
                    */

                    if(line[index] == ':' && line[index + 1] == 'a') {
                        if(line[index + 2] == '{') {
                            bytes.push_back(0x05);
                        } else if(line[index + 2] == '}') {
                            bytes.push_back(0x06);
                        }
                    }

                    /*
                    * Operator name: Long integer [0x17]
                    * Operands: Integer
                    * Format: %i(value)
                    *
                    * Algorithm:
                    * Check if the next sequence is "%i("
                    * Increase index by 3
                    * Read the value until dot
                    * Convert it to long
                    * Skip leftovers
                    */
                    if(line[index] == '%' && line[index + 1] == 'i' && line[index + 2] == '(') {
                        index += 3;

                        std::string valueString;

                        while(index < line.size()) {
                            if(line[index] == '.') {
                                break;
                            }

                            valueString.push_back(line[index]);

                            index++;
                        }

                        while(index < line.size()) {
                            if(line[index] == ')') {
                                break;
                            }

                            index++;
                        }

                        long value = std::stol(valueString);

                        bytes.push_back(0x17);

                        bytes.push_back(value);
                        bytes.push_back((value) >> 8);
                        bytes.push_back((value) >> 16);
                        bytes.push_back((value) >> 24);
                    }

                    /*
                    * Operator name: Float [0x1A]
                    * Operands: Float
                    * Format: %f(value)
                    *
                    * Algorithm:
                    * Check if the next sequence is "%f("
                    * Increase index by 3
                    * Read the value until )
                    * Convert it to float
                    */
                    if(line[index] == '%' && line[index + 1] == 'f' && line[index + 2] == '(') {
                        index += 3;

                        std::string valueString;

                        while(index < line.size()) {
                            if(line[index] == ')') {
                                break;
                            }

                            valueString.push_back(line[index]);

                            index++;
                        }

                        float value = std::stof(valueString);
                        unsigned char valueBytes[4];

                        std::memcpy(valueBytes, reinterpret_cast<const unsigned char*>(&value), sizeof(float));

                        bytes.push_back(0x1A);

                        bytes.push_back(valueBytes[0]);
                        bytes.push_back(valueBytes[1]);
                        bytes.push_back(valueBytes[2]);
                        bytes.push_back(valueBytes[3]);
                    }

                    /*
                    * Operator name: String [0x1B]
                    * Operands: Integer, String
                    * Format: %s(size, value)
                    *
                    * Algorithm:
                    * Check if the next sequence is "%s("
                    * Increase index by 3
                    * Skip until the "
                    * Read the value until "
                    * Increase index by 1
                    * Calculate it's size
                    */
                    if(line[index] == '%' && line[index + 1] == 's' && line[index + 2] == '(') {
                        index += 3;

                        std::string value;

                        while(index < line.size()) {
                            if(line[index] == '"') {
                                index++;
                                break;
                            }

                            index++;
                        }

                        while(index < line.size()) {
                            if(line[index] == '"') {
                                break;
                            }

                            value.push_back(line[index]);

                            index++;
                        }

                        index += 1;

                        unsigned long valueSize = value.size() + 1;

                        bytes.push_back(0x1B);

                        bytes.push_back(valueSize);
                        bytes.push_back((valueSize) >> 8);
                        bytes.push_back((valueSize) >> 16);
                        bytes.push_back((valueSize) >> 24);

                        for(char c : value) {
                            bytes.push_back(c);
                        }

                        bytes.push_back(0x00);
                    }
                }

                /*
                * Operator name: Checksum [0x16]
                * Operands: Integer
                * Format: $[hex number]$
                *
                * Algorithm:
                * Find a dollar sign with opening bracket
                * Increase the index by 2 to skip them
                * Loop read until closing bracket is found
                * Increase index by 2
                * Add the checksum instruction
                * Increase the index again to skip the last dollar sign
                */
                if (line[index] == '$' && line[index + 1] == '[') {
                    index += 2;

                    std::string hex_string;

                    if(index + 8 > line.size()) {
                        break;
                    }

                    for (int i = 0; i < 8; i++) {
                        hex_string.push_back(line[index]);

                        printf("%c\n", line[index]);

                        index++;
                    }

                    index += 2;

                    unsigned long checksum = std::stol(hex_string, nullptr, 16);

                    bytes.push_back(0x16);

                    bytes.push_back(((checksum) >> 24) & 0xFF);
                    bytes.push_back(((checksum) >> 16) & 0xFF);
                    bytes.push_back(((checksum) >> 8) & 0xFF);
                    bytes.push_back(checksum & 0xFF);

                    continue;
                }
            }

            /*
            * Operator name: Checksum name [0x16]
            * Operands: Text
            * Format: $name$
            *
            * Algorithm:
            * Find a dollar sign
            * Increase the index by 1 to skip it
            * Loop read until another $ is found
            * Calculate the checksum of the name
            * Add the checksum instruction
            * Increase the index again to skip the last dollar sign
            */
            if (line[index] == '$') {
                std::string checksum_name;
                index++;

                while (line.size() > index) {
                    if (line[index] == '$') {
                        break;
                    }

                    checksum_name.push_back(line[index]);

                    index++;
                }

                unsigned long checksum = qb_crc::generate(checksum_name);

                bytes.push_back(0x16);

                bytes.push_back(checksum & 0xFF);
                bytes.push_back(((checksum) >> 8) & 0xFF);
                bytes.push_back(((checksum) >> 16) & 0xFF);
                bytes.push_back(((checksum) >> 24) & 0xFF);

                index++;

                continue;
            }

            /*
            * Operator name: Equals [0x07]
            * Operands: None
            * Format: =
            *
            * Algorithm:
            * Insert if the next character is '='
            */
            if(line[index] == '=') {
                bytes.push_back(0x07);
            }

            /*
            * Operator name: Dot [0x08]
            * Operands: None
            * Format: .
            *
            * Algorithm:
            * Insert if the next character is '.'
            */
            if(line[index] == '.') {
                bytes.push_back(0x08);
            }

            /*
            * Operator name: Comma [0x09]
            * Operands: None
            * Format: ,
            *
            * Algorithm:
            * Insert if the next character is ','
            */
            if(line[index] == ',') {
                bytes.push_back(0x09);
            }

            /*
            * Operator name: Minus [0xA]
            * Operands: None
            * Format: -
            *
            * Algorithm:
            * Insert if the next character is '-'
            */
            if(line[index] == '-') {
                bytes.push_back(0xA);
            }

            /*
            * Operator name: Plus [0xB]
            * Operands: None
            * Format: +
            *
            * Algorithm:
            * Insert if the next character is '+'
            */
            if(line[index] == '+') {
                bytes.push_back(0xB);
            }

            /*
            * Operator name: Multiply [0xC]
            * Operands: None
            * Format: *
            *
            * Algorithm:
            * Insert if the next character is '*'
            */
            if(line[index] == '*') {
                bytes.push_back(0xC);
            }

            /*
            * Operator name: Divide [0xD]
            * Operands: None
            * Format: /
            *
            * Algorithm:
            * Insert if the next character is '/'
            */
            if(line[index] == '/') {
                bytes.push_back(0xD);
            }

            /*
            * Operator name: Less than [0x12]
            * Operands: None
            * Format: <
            *
            * Algorithm:
            * Insert if the next character is '<'
            */
            if(line[index] == '<') {
                bytes.push_back(0x12);
            }

            /*
            * Operator name: More than [0x14]
            * Operands: None
            * Format: >
            *
            * Algorithm:
            * Insert if the next character is '>'
            */
            if(line[index] == '>') {
                bytes.push_back(0x14);
            }

            index++;
        }
    }

    return bytes;
}

float qb_recompiler::readFloat(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset];
    u8 b2 = bytes[offset + 1];
    u8 b3 = bytes[offset + 2];
    u8 b4 = bytes[offset + 3];

    float value;
    u8 value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    return value;
}

int qb_recompiler::readInt(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset];
    u8 b2 = bytes[offset + 1];
    u8 b3 = bytes[offset + 2];
    u8 b4 = bytes[offset + 3];

    int value;
    u8 value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    return value;
}

int qb_recompiler::readInvInt(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset + 3];
    u8 b2 = bytes[offset + 2];
    u8 b3 = bytes[offset + 1];
    u8 b4 = bytes[offset];

    int value;
    u8 value_bytes[] = {b1, b2, b3, b4};
    memcpy(&value, &value_bytes, sizeof(int32_t));

    return value;
}

short qb_recompiler::readShort(size_t offset, std::vector<u8> bytes) {
    u8 b1 = bytes[offset];
    u8 b2 = bytes[offset + 1];

    short value;
    u8 value_bytes[] = {b1, b2};
    memcpy(&value, &value_bytes, sizeof(short));
    return value;
}