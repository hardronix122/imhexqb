#ifndef IMHEX_QB_DECOMPILER_H
#define IMHEX_QB_DECOMPILER_H

#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <exception>
#include "hex/helpers/types.hpp"
#include "qb_crc.h"
#include "exception/qb_exception.h"

class qb_recompiler {
public:
    static std::string decompile(std::vector<u8> bytes, std::map<int32_t, std::string> &symbols, bool greedySymbolCapture, bool heuristicIndentation);
    static std::vector<u8> compile(std::string& source);
    static float readFloat(size_t offset, std::vector<u8> bytes);
    static int readInt(size_t offset, std::vector<u8> bytes);
    static int readInvInt(size_t offset, std::vector<u8> bytes);
    static short readShort(size_t offset, std::vector<u8> bytes);
};


#endif