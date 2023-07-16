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
                code += " == ";
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
                    u8 b1 = bytes[i];
                    u8 b2 = bytes[i + 1];
                    u8 b3 = bytes[i + 2];
                    u8 b4 = bytes[i + 3];

                    int checksum = int(b1 << 24 | b2 << 16 | b3 << 8 | b4);

                    code += "$[" + std::to_string(checksum) + "]$";

                    i += 4;
                }

                break;
            case 0x17: // Long value
                if (i + 4 <= bytes.size()) {
                    u8 b1 = bytes[i];
                    u8 b2 = bytes[i + 1];
                    u8 b3 = bytes[i + 2];
                    u8 b4 = bytes[i + 3];

                    long checksum = long(b1 << 24 | b2 << 16 | b3 << 8 | b4);

                    code += "%i(" + std::to_string(checksum) + ".00000000)";

                    i += 4;
                }
                break;
            default:
                code += " <[!Unknown Instruction " + std::to_string(byte) + "!]> ";
                break;
        }
    }

    return code;
}
