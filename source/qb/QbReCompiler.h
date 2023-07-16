#ifndef IMHEX_QB_DECOMPILER_H
#define IMHEX_QB_DECOMPILER_H

#include <vector>
#include <string>
#include <format>
#include <iostream>
#include <iomanip>
#include "hex/helpers/types.hpp"

class QbReCompiler {
public:
    static std::string decompile(std::vector<u8> bytes);

    static float readFloat(size_t offset, std::vector<u8> bytes);
    static int readInt(size_t offset, std::vector<u8> bytes);
};


#endif