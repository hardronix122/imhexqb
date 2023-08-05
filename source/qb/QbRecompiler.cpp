#include "QbRecompiler.h"

std::string
QbRecompiler::decompile(std::vector<u8> bytes, std::map<int32_t, std::string> &symbols, bool greedySymbolCapture,
                        bool heuristicIndentation, int generation) {
    std::string code;

    if (bytes[0] != 0x01 && generation > 4) {
        code += "#/ Owops! This chunk doesn't start with a new instruction!\n#/ Not big of a deal if you know what you're actually doing, just wanna let ya know :3\n";
    }

    int i = 0;
    u8 lastInstruction;
    bool insideCase;

    while (i < bytes.size()) {
        u8 byte = bytes[i];

        i++;

        switch (byte) {
            case 0x01: // New Instruction
                code += "\n:i ";

                // If we're dealing with THPS4 or lower, then skip 2 additional bytes
                if (generation < 5) {
                    i += 2;
                }

                break;
            case 0x02: // End Of Line
                // Add a semicolon if dealing with THPS4 or lower
                if (generation < 5) {
                    code += ";\n";
                }

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

                if (heuristicIndentation) {
                    code += "\n";
                }

                break;
            case 0x06: // End Array
                if (heuristicIndentation) {
                    code += "\n";
                }

                code += ":a}";

                if (heuristicIndentation) {
                    code += "\n";
                }

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
            case 0xE: // Open parenthesis
            case 0xF: // Close parenthesis
                break;
            case 0x10: // Never? Whatever..
                code += "<[!This instruction shouldn't be here!]>";
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
                    int checksum = DataHelper::readReversedInteger(i, bytes);

                    if (symbols.contains(checksum)) {
                        code += "$" + symbols[checksum] + "$";
                    } else {
                        std::stringstream ss;
                        ss << std::setfill('0') << std::setw(sizeof(int) * 2) << std::hex << checksum;
                        code += "$[" + ss.str() + "]$";
                    }

                    // Add a new line if last instruction was function (0x23)
                    if (heuristicIndentation && lastInstruction == 0x23) {
                        code += "\n";
                    }
                }

                break;
            case 0x17: // Long integer
                if (i + 4 <= bytes.size()) {
                    int value = DataHelper::readInteger(i, bytes);

                    code += "%i(" + std::to_string(value) + ",00000000)";
                }
                break;
            case 0x18: // Hex Integer
                if (i + 4 <= bytes.size()) {
                    int value = DataHelper::readInteger(i, bytes);

                    code += "%i(" + std::to_string(value) + ",";

                    std::stringstream ss;
                    ss << std::setfill('0') << std::setw(sizeof(int) * 2) << std::hex << value;

                    code += ss.str() + ')';
                }
                break;
            case 0x19: // TODO: Implement enum
                break;
            case 0x1A: // Single float
                if (i + 4 <= bytes.size()) {
                    float value = DataHelper::readFloat(i, bytes);

                    code += "%f(" + std::to_string(value) + ")";

                    i += 4;
                }

                break;
            case 0x1B: // String
            case 0x1C: // String
                if (i + 4 <= bytes.size()) {
                    int size = DataHelper::readInteger(i, bytes);

                    code += "%s(" + std::to_string(size) + ", ";

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
                    float x = DataHelper::readFloat(i, bytes);
                    i += 4;
                    float y = DataHelper::readFloat(i, bytes);
                    i += 4;
                    float z = DataHelper::readFloat(i, bytes);
                    i += 4;

                    code += "%vec3(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
                }

                break;
            case 0x1F: // Pair
                if (i + 8 <= bytes.size()) {
                    float x = DataHelper::readFloat(i, bytes);
                    i += 4;
                    float y = DataHelper::readFloat(i, bytes);
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
                if (heuristicIndentation) {
                    code += "\n";
                }

                code += "function ";
                break;
            case 0x24: // End Function
                if (heuristicIndentation) {
                    code += "\n";
                }

                code += "endfunction";

                if (heuristicIndentation) {
                    code += "\n\n";
                }

                break;
            case 0x25: // If
                code += "if ";
                break;
            case 0x26: // Else
                if (heuristicIndentation) {
                    code += "\n";
                }

                code += "else";
                break;
            case 0x27: // Elseif
                if (heuristicIndentation) {
                    code += "\n";
                }

                code += "elseif";
                break;
            case 0x28: // Endif
                if (heuristicIndentation) {
                    code += "\n";
                }

                code += "endif";

                if (heuristicIndentation) {
                    code += "\n";
                }

                break;
            case 0x29: // Return
                if (heuristicIndentation) {
                    code += "\n";
                }

                code += "return";
                break;
            case 0x2A: // Undefined
                break;
            case 0x2B: // Symbol entry
                if (i + 4 <= bytes.size()) {
                    int checksum = DataHelper::readReversedInteger(i, bytes);

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

                if (heuristicIndentation) {
                    code += "\n";
                }

                break;
            case 0x2D: // Argument stack or Global
                code += "%GLOBAL%";
                break;
            case 0x2E: // Jump
                if (i + 4 <= bytes.size()) {
                    code += "jump[";
                    int offset = DataHelper::readInteger(i, bytes);
                    code += std::to_string(offset);
                    code += "]";
                }
                break;
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
            case 0x34: // XOR condition
                code += " XOR ";
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
                code += " NOT ";
                break;
            case 0x3C: // Switch expression
                if (heuristicIndentation) {
                    code += "\n";
                }

                code += "switch ";
                break;
            case 0x3D: // End Switch expression
                if (heuristicIndentation) {
                    code += "\n";
                }

                insideCase = false;

                code += "end_switch";
                break;
            case 0x3E: // Case expression
                if (heuristicIndentation) {
                    code += "\n";
                }

                if (insideCase) {
                    if (lastInstruction != 0x01 && lastInstruction != 0x49) {
                        code += " ";
                    }

                    code += "endcase\n";
                }

                insideCase = true;

                code += "case ";
                break;
            case 0x3F: // Default case expression
                code += "default";

                if (heuristicIndentation) {
                    code += "\n";
                }

                break;
            case 0x40: // Random (no repeat)
                code += "no_repeat_random";
                break;
            case 0x41: // Random permute
                code += "random_permute";
                break;
            case 0x42: // Colon
                code += ".";
                break;
            case 0x43: // Runtime c-function
                code += " c_function ";
            case 0x44: // Runtime member function
                code += " member_function ";
            case 0x45: // TODO: Unknown
            case 0x46: // TODO: Unknown
                break;
            case 0x47: // If expression with short offset
                if (i + 2 <= bytes.size()) {
                    short offset = DataHelper::readShort(i, bytes); // TODO: Figure out what that offset is used for
                    i += 2;

                    code += "if[" + std::to_string(offset) + "] ";
                }

                break;
            case 0x48: // Else expression with short offset
                if (i + 2 <= bytes.size()) {
                    short offset = DataHelper::readShort(i, bytes); // TODO: Figure out what that offset is used for
                    i += 2;

                    code += "else[" + std::to_string(offset) + "] ";

                    if (heuristicIndentation) {
                        code += "\n";
                    }
                }

                break;
            case 0x49: // Short break
                if (i + 2 <= bytes.size()) {
                    short offset = DataHelper::readShort(i, bytes); // TODO: Figure out what that offset is used for
                    i += 2;

                    code += "shortbreak[" + std::to_string(offset) + "] ";
                }

                break;
            default:
                code += " <[!Unknown Instruction " + std::to_string(byte) + "!]> ";
                break;
        }

        lastInstruction = byte;
    }

    return code;
}

std::vector<u8> QbRecompiler::compile(std::string &source, int generation) {
    std::vector<u8> bytes = std::vector<u8>();

    std::istringstream sourceStream(source);
    std::string line;
    int lineIndex = 1;

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

                    // If we're dealing with THPS4 or lower, then we will also need to add 2 additional bytes
                    if (generation < 5) {
                        bytes.push_back(0x00);
                        bytes.push_back(0x00);
                    }
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

                if (line.size() >= 3) {
                    // Make sure there are no de-compilation warnings
                    if (line[index] == '<' && line[index + 1] == '[' && line[index + 2] == '!') {
                        throw QbException("De-compilation warning found!");
                    }

                    if (line[index] == '!' && line[index + 1] == ']' && line[index + 2] == '>') {
                        throw QbException("De-compilation warning found!");
                    }

                    /*
                    * Operator name: Struct [0x03] + Struct End [0x04}
                    * Operands: None
                    * Format: :s{ / :s}
                    *
                    * Algorithm:
                    * Insert struct begin if the next sequence is :s{, insert struct end if the next sequence is :s}
                    */

                    if (line[index] == ':' && line[index + 1] == 's') {
                        if (line[index + 2] == '{') {
                            bytes.push_back(0x03);
                        } else if (line[index + 2] == '}') {
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

                    if (line[index] == ':' && line[index + 1] == 'a') {
                        if (line[index + 2] == '{') {
                            bytes.push_back(0x05);
                        } else if (line[index + 2] == '}') {
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
                    * Read the value until comma
                    * Convert it to long
                    * Skip leftovers
                    */
                    if (line[index] == '%' && line[index + 1] == 'i' && line[index + 2] == '(') {
                        index += 3;

                        std::string valueString;

                        while (index < line.size()) {
                            if (line[index] == ',') {
                                break;
                            }

                            valueString.push_back(line[index]);

                            index++;
                        }

                        while (index < line.size()) {
                            if (line[index] == ')') {
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
                    if (line[index] == '%' && line[index + 1] == 'f' && line[index + 2] == '(') {
                        index += 3;

                        std::string valueString;

                        while (index < line.size()) {
                            if (line[index] == ')') {
                                break;
                            }

                            valueString.push_back(line[index]);

                            index++;
                        }

                        float value = std::stof(valueString);
                        unsigned char valueBytes[4];

                        std::memcpy(valueBytes, reinterpret_cast<const unsigned char *>(&value), sizeof(float));

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
                    if (line[index] == '%' && line[index + 1] == 's' && line[index + 2] == '(') {
                        index += 3;

                        std::string value;

                        while (index < line.size()) {
                            if (line[index] == '"') {
                                index++;
                                break;
                            }

                            index++;
                        }

                        while (index < line.size()) {
                            if (line[index] == '"') {
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

                        for (char c: value) {
                            bytes.push_back(c);
                        }

                        bytes.push_back(0x00);
                    }
                }


                if (line.size() >= 6) {
                    /*
                    * Operator name: Vector [0x1E]
                    * Operands: Float, Float, Float
                    * Format: %vec3(value, value, value)
                    *
                    * Algorithm:
                    * Check if the next sequence is "%vec3("
                    * Increase the index by 6
                    * Read the first value until comma
                    * Increase the index by 1
                    * Read the second value until comma
                    * Increase the index by 1
                    * Read the third value until ')'
                    */

                    // TODO: Probably replace it with substr comparison, questioning my own existence right now
                    if (line[index] == '%' && line[index + 1] == 'v' && line[index + 2] == 'e' &&
                        line[index + 3] == 'c' && line[index + 4] == '3' && line[index + 5] == '(') {
                        index += 6;

                        float x;
                        float y;
                        float z;

                        std::string tempValue;

                        while (index < line.size()) {
                            if (line[index] == ',') {
                                index++;
                                break;
                            }

                            tempValue.push_back(line[index]);

                            index++;
                        }

                        x = std::stof(tempValue);

                        tempValue.clear();

                        while (index < line.size()) {
                            if (line[index] == ',') {
                                index++;
                                break;
                            }

                            tempValue.push_back(line[index]);

                            index++;
                        }

                        y = std::stof(tempValue);

                        tempValue.clear();

                        while (index < line.size()) {
                            if (line[index] == ')') {
                                break;
                            }

                            tempValue.push_back(line[index]);

                            index++;
                        }

                        z = std::stof(tempValue);

                        tempValue.clear();

                        unsigned char xBytes[4];
                        unsigned char yBytes[4];
                        unsigned char zBytes[4];

                        std::memcpy(xBytes, reinterpret_cast<const unsigned char *>(&x), sizeof(float));
                        std::memcpy(yBytes, reinterpret_cast<const unsigned char *>(&y), sizeof(float));
                        std::memcpy(zBytes, reinterpret_cast<const unsigned char *>(&z), sizeof(float));

                        bytes.push_back(0x1E);

                        bytes.push_back(xBytes[0]);
                        bytes.push_back(xBytes[1]);
                        bytes.push_back(xBytes[2]);
                        bytes.push_back(xBytes[3]);

                        bytes.push_back(yBytes[0]);
                        bytes.push_back(yBytes[1]);
                        bytes.push_back(yBytes[2]);
                        bytes.push_back(yBytes[3]);

                        bytes.push_back(zBytes[0]);
                        bytes.push_back(zBytes[1]);
                        bytes.push_back(zBytes[2]);
                        bytes.push_back(zBytes[3]);
                    }

                    /*
                    * Operator name: Pair [0x1F]
                    * Operands: Float, Float
                    * Format: %vec2(value, value)
                    *
                    * Algorithm:
                    * Check if the next sequence is "%vec2("
                    * Increase the index by 6
                    * Read the first value until comma
                    * Increase the index by 1
                    * Read the second value until ')'
                    */

                    // TODO: Probably replace it with substr comparison too, questioning my own existence right now even more
                    if (line[index] == '%' && line[index + 1] == 'v' && line[index + 2] == 'e' &&
                        line[index + 3] == 'c' && line[index + 4] == '2' && line[index + 5] == '(') {
                        index += 6;

                        float x;
                        float y;

                        std::string tempValue;

                        while (index < line.size()) {
                            if (line[index] == ',') {
                                index++;
                                break;
                            }

                            tempValue.push_back(line[index]);

                            index++;
                        }

                        x = std::stof(tempValue);

                        tempValue.clear();

                        while (index < line.size()) {
                            if (line[index] == ')') {
                                break;
                            }

                            tempValue.push_back(line[index]);

                            index++;
                        }

                        y = std::stof(tempValue);

                        tempValue.clear();

                        unsigned char xBytes[4];
                        unsigned char yBytes[4];

                        std::memcpy(xBytes, reinterpret_cast<const unsigned char *>(&x), sizeof(float));
                        std::memcpy(yBytes, reinterpret_cast<const unsigned char *>(&y), sizeof(float));

                        bytes.push_back(0x1F);

                        bytes.push_back(xBytes[0]);
                        bytes.push_back(xBytes[1]);
                        bytes.push_back(xBytes[2]);
                        bytes.push_back(xBytes[3]);

                        bytes.push_back(yBytes[0]);
                        bytes.push_back(yBytes[1]);
                        bytes.push_back(yBytes[2]);
                        bytes.push_back(yBytes[3]);
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

                    if (index + 8 > line.size()) {
                        break;
                    }

                    for (int i = 0; i < 8; i++) {
                        hex_string.push_back(line[index]);
                        index++;
                    }

                    index += 2;

                    unsigned long checksum = std::stoul(hex_string, nullptr, 16);

                    bytes.push_back(0x16);

                    bytes.push_back(((checksum) >> 24) & 0xFF);
                    bytes.push_back(((checksum) >> 16) & 0xFF);
                    bytes.push_back(((checksum) >> 8) & 0xFF);
                    bytes.push_back(checksum & 0xFF);

                    continue;
                }

                /*
                * Operator name: AllArgs [0x2C]
                * Operands: None
                * Format: isNull
                * Algorithm:
                * Insert if the next sequence is "isNull"
                */
                if (line.substr(index, 6) == "isNull") {
                    bytes.push_back(0x2C);
                    index += 5;
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

                unsigned long checksum = QbCrc::generate(checksum_name);

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
            if (line[index] == '=') {
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
            if (line[index] == '.') {
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
            if (line[index] == ',') {
                bytes.push_back(0x09);
            }

            /*
            * Operator name: Minus [0x0A]
            * Operands: None
            * Format: -
            *
            * Algorithm:
            * Insert if the next character is '-'
            */
            if (line[index] == '-') {
                bytes.push_back(0xA);
            }

            /*
            * Operator name: Plus [0x0B]
            * Operands: None
            * Format: +
            *
            * Algorithm:
            * Insert if the next character is '+'
            */
            if (line[index] == '+') {
                bytes.push_back(0xB);
            }

            /*
            * Operator name: Multiply [0x0C]
            * Operands: None
            * Format: *
            *
            * Algorithm:
            * Insert if the next character is '*'
            */
            if (line[index] == '*') {
                bytes.push_back(0xC);
            }

            /*
            * Operator name: Divide [0x0D]
            * Operands: None
            * Format: /
            *
            * Algorithm:
            * Insert if the next character is '/'
            */
            if (line[index] == '/') {
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
            if (line[index] == '<') {
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
            if (line[index] == '>') {
                bytes.push_back(0x14);
            }

            if (index + 8 <= line.size()) {
                /*
                * Operator name: Function [0x23]
                * Operands: None
                * Format: function
                *
                * Algorithm:
                * Insert if the next sequence is "function"
                */
                if (line.substr(index, 8) == "function") {
                    bytes.push_back(0x23);
                    index += 7;
                }

                /*
                * Operator name: ArgStack / Global [0x2D]
                * Operands: None
                * Format: %GLOBAL%
                *
                * Algorithm:
                * Insert if the next sequence is %GLOBAL%
                */
                if (line.substr(index, 8) == "%GLOBAL%") {
                    bytes.push_back(0x2D);
                    index += 7;
                }
            }

            if (index + 11 <= line.size()) {
                /*
                * Operator name: End Function [0x24]
                * Operands: None
                * Format: endfunction
                *
                * Algorithm:
                * Insert if the next sequence is "endfunction"
                */

                if (line.substr(index, 11) == "endfunction") {
                    bytes.push_back(0x24);
                    index += 10;
                }

                /*
                * Operator name: Short Break [0x49]
                * Operands: Offset
                * Format: shortbreak[offset]
                *
                * Algorithm:
                * Increase index by 11 to skip shortbreak[
                * Read offset until ]
                */

                if (line.substr(index, 11) == "shortbreak[") {
                    index += 11;

                    std::string stringOffset;

                    while (line.size() > index) {
                        if (line[index] == ']') {
                            break;
                        }

                        stringOffset.push_back(line[index]);

                        index++;
                    }

                    auto offset = (short) std::stoi(stringOffset);

                    // TODO: Temporary solution for debugging purposes
                    bytes.push_back(0x49);

                    bytes.push_back(offset & 0xFF);
                    bytes.push_back((offset >> 8) & 0xFF);

                }
            }

            if (index + 3 <= line.size()) {
                /*
                * Operator name: If with offset [0x47]
                * Operands: Offset
                * Format: if[offset]
                *
                * Algorithm:
                * Increase index by 3 to skip if[
                * Read the offset until ']'
                */
                if (line[index] == 'i' && line[index + 1] == 'f' && line[index + 2] == '[') {
                    index += 3;

                    std::string stringOffset;

                    while (line.size() > index) {
                        if (line[index] == ']') {
                            break;
                        }

                        stringOffset.push_back(line[index]);

                        index++;
                    }

                    auto offset = (short) std::stoi(stringOffset);

                    // TODO: Temporary solution for debugging purposes
                    bytes.push_back(0x47);

                    bytes.push_back(offset & 0xFF);
                    bytes.push_back((offset >> 8) & 0xFF);
                }

                /*
                * Operator name: Conditional NOT [0x39]
                * Operands: None
                * Format: NOT
                *
                * Algorithm:
                * Insert if the next sequence is "NOT"
                */
                if (line[index] == 'N' && line[index + 1] == 'O' && line[index + 2] == 'T') {
                    bytes.push_back(0x39);

                    index += 2;
                }

                /*
                * Operator name: Conditional AND [0x3A]
                * Operands: None
                * Format: AND
                *
                * Algorithm:
                * Insert if the next sequence is "AND"
                */
                if (line[index] == 'A' && line[index + 1] == 'N' && line[index + 2] == 'D') {
                    bytes.push_back(0x3A);

                    index += 2;
                }
            }

            if (index + 2 <= line.size()) {
                /*
                * Operator name: If [0x25]
                * Operands: None
                * Format: if
                *
                * Algorithm:
                * Insert if the next sequence is "if"
                */
                if (line[index] == 'i' && line[index + 1] == 'f') {
                    bytes.push_back(0x25);

                    index += 1;
                }

                /*
                * Operator name: Conditional OR [0x3B]
                * Operands: None
                * Format: OR
                *
                * Algorithm:
                * Insert if the next sequence is "OR"
                */
                if (line[index] == 'O' && line[index + 1] == 'R') {
                    bytes.push_back(0x3B);

                    index += 1;
                }
            }

            if (index + 5 <= line.size()) {
                /*
                * Operator name: Jump [0x2E]
                * Operands: Offset
                * Format: jump[offset]
                *
                * Algorithm:
                * Increase index by 5
                * Read offset till ']'
                */
                if (line[index] == 'j' && line[index + 1] == 'u' && line[index + 2] == 'm' && line[index + 3] == 'p' &&
                    line[index + 4] == '[') {
                    index += 5;

                    std::string stringOffset;

                    while (index < line.size()) {
                        if (line[index] == ']') {
                            break;
                        }

                        stringOffset.push_back(line[index]);

                        index++;
                    }

                    auto offset = (short) std::stoi(stringOffset);

                    bytes.push_back(0x2E);

                    bytes.push_back(offset & 0xFF);
                    bytes.push_back((offset >> 8) & 0xFF);
                }

                /*
                * Operator name: Break [0x22]
                * Operands: None
                * Format: break
                *
                * Algorithm:
                * Insert if the next sequence is "break"
                */
                if (line[index] == 'b' && line[index + 1] == 'r' && line[index + 2] == 'e' && line[index + 3] == 'a' &&
                    line[index + 4] == 'k') {
                    bytes.push_back(0x22);

                    index += 4;
                }

                /*
                * Operator name: While [0x20]
                * Operands: None
                * Format: while
                *
                * Algorithm:
                * Insert if the next sequence is "while"
                */

                if (line[index] == 'w' && line[index + 1] == 'h' && line[index + 2] == 'i' && line[index + 3] == 'l' &&
                    line[index + 4] == 'e') {
                    bytes.push_back(0x20);

                    index += 4;
                }

                /*
                * Operator name: Else with offset [0x48]
                * Operands: Offset
                * Format: else[offset]
                *
                * Algorithm:
                * Increase the index by 5 to skip else[
                * Read an offset till ']'
                */
                if (line[index] == 'e' && line[index + 1] == 'l' && line[index + 2] == 's' && line[index + 3] == 'e' &&
                    line[index + 4] == '[') {
                    index += 5;

                    std::string stringOffset;

                    while (index < line.size()) {
                        if (line[index] == ']') {
                            break;
                        }

                        stringOffset.push_back(line[index]);

                        index++;
                    }

                    auto offset = (short) std::stoi(stringOffset);

                    bytes.push_back(0x48);

                    bytes.push_back(offset & 0xFF);
                    bytes.push_back((offset >> 8) & 0xFF);
                }

                /*
                * Operator name: If End [0x28]
                * Operands: None
                * Format: endif
                *
                * Algorithm:
                * Insert if the next sequence is "endif"
                */
                if (line[index] == 'e' && line[index + 1] == 'n' && line[index + 2] == 'd' && line[index + 3] == 'i' &&
                    line[index + 4] == 'f') {
                    bytes.push_back(0x28);

                    index += 4;
                }
            }

            if (index + 7 <= line.size()) {
                /*
                * Operator name: Repeat [0x21]
                * Operands: None
                * Format: loop_to
                *
                * Algorithm:
                * Insert if the next sequence is "loop_to"
                */

                if (line[index] == 'l' && line[index + 1] == 'o' && line[index + 2] == 'o' && line[index + 3] == 'p' &&
                    line[index + 4] == '_' && line[index + 5] == 't' && line[index + 6] == 'o') {
                    bytes.push_back(0x21);

                    index += 6;
                }

                /*
                * Operator name: Default Case [0x3F]
                * Operands: None
                * Format: default
                *
                * Algorithm:
                * Insert if the next sequence is "default"
                */

                if (line[index] == 'd' && line[index + 1] == 'e' && line[index + 2] == 'f' && line[index + 3] == 'a' &&
                    line[index + 4] == 'u' && line[index + 5] == 'l' && line[index + 6] == 't') {
                    bytes.push_back(0x3F);

                    index += 6;
                }

                /*
                * Operator name: End Case [Internal]
                * Operands: None
                *
                * Algorithm:
                * Skip if the next sequence is "endcase"
                */

                if (line[index] == 'e' && line[index + 1] == 'n' && line[index + 2] == 'd' && line[index + 3] == 'c' &&
                    line[index + 4] == 'a' && line[index + 5] == 's' && line[index + 6] == 'e') {
                    index += 6;
                }
            }

            if (index + 6 <= line.size()) {
                /*
                * Operator name: Else If [0x27]
                * Operands: None
                * Format: elseif
                *
                * Algorithm:
                * Insert if the next sequence is "elseif"
                */

                if (line[index] == 'e' && line[index + 1] == 'l' && line[index + 2] == 's' && line[index + 3] == 'e' &&
                    line[index + 4] == 'i' && line[index + 5] == 'f') {
                    bytes.push_back(0x27);

                    index += 5;
                }

                /*
                * Operator name: Switch [0x3C]
                * Operands: None
                * Format: switch
                *
                * Algorithm:
                * Insert if the next sequence is "switch"
                */
                if (line[index] == 's' && line[index + 1] == 'w' && line[index + 2] == 'i' && line[index + 3] == 't' &&
                    line[index + 4] == 'c' && line[index + 5] == 'h') {
                    bytes.push_back(0x3C);

                    index += 5;
                }
            }

            if (index + 4 <= line.size()) {
                /*
                * Operator name: Else [0x26]
                * Operands: None
                * Format: else
                *
                * Algorithm:
                * Insert if the next sequence is "else"
                */
                if (line[index] == 'e' && line[index + 1] == 'l' && line[index + 2] == 's' && line[index + 3] == 'e') {
                    bytes.push_back(0x26);

                    index += 3;
                }

                /*
                * Operator name: Case [0x3E]
                * Operands: None
                * Format: case
                *
                * Algorithm:
                * Insert if the next sequence is "case"
                */
                if (line[index] == 'c' && line[index + 1] == 'a' && line[index + 2] == 's' && line[index + 3] == 'e') {
                    bytes.push_back(0x3E);

                    index += 3;
                }
            }

            if (index + 10 <= line.size()) {
                /*
                * Operator name: End Switch [0x3D]
                * Operands: None
                * Format: end_switch
                *
                * Algorithm:
                * Insert if the next sequence is "end_switch"
                */
                if (line[index] == 'e' && line[index + 1] == 'n' && line[index + 2] == 'd' && line[index + 3] == '_' &&
                    line[index + 4] == 's' && line[index + 5] == 'w' && line[index + 6] == 'i' &&
                    line[index + 7] == 't' && line[index + 8] == 'c' && line[index + 9] == 'h') {

                    bytes.push_back(0x3D);

                    index += 9;
                }
            }

            /*
            * Operator name: End Of Line [0x02]
            * Operands: None
            * Format: ;
            *
            * Algorithm:
            * Insert if the next symbol is ';'
            */
            if (line[index] == ';') {
                // Then insert the end of line instruction
                bytes.push_back(0x02);

                bytes.push_back(lineIndex);
                bytes.push_back((lineIndex) >> 8);
                bytes.push_back((lineIndex) >> 16);
                bytes.push_back((lineIndex) >> 24);
            }

            /*
            * Operator name: Open parenthesis [0x0E]
            * Operands: None
            * Format: (
            *
            * Algorithm:
            * Insert if the next symbol is '('
            */
            if (line[index] == '(') {
                bytes.push_back(0x0E);
            }

            /*
            * Operator name: Close parenthesis [0x0F]
            * Operands: None
            * Format: )
            *
            * Algorithm:
            * Insert if the next symbol is ')'
            */
            if (line[index] == ')') {
                bytes.push_back(0x0F);
            }

            index++;
        }

        lineIndex++;
    }

    return bytes;
}