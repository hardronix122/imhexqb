#ifndef IMHEX_QB_DECOMPILER_H
#define IMHEX_QB_DECOMPILER_H

#include <vector>
#include <string>
#include <map>
#include "hex/helpers/types.hpp"

class QbReCompiler {
public:
    static std::string decompile(std::vector<u8> bytes, std::map<int32_t, std::string> &symbols, bool greedySymbolCapture);

    static float readFloat(size_t offset, std::vector<u8> bytes);
    static int readInt(size_t offset, std::vector<u8> bytes);
    static short readShort(size_t offset, std::vector<u8> bytes);
};


#endif