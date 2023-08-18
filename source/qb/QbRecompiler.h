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
#include "QbCrc.h"
#include "../helper/DataHelper.h"
#include "exception/QbException.h"
#include "ChecksumDictionary.h"

class QbRecompiler {
public:
    static std::string decompile(std::vector<u8> bytes, ChecksumDictionary &symbols, bool greedySymbolCapture, bool heuristicIndentation, int generation);
    static std::vector<u8> compile(std::string& source, ChecksumDictionary& symbols, int generation);
};


#endif