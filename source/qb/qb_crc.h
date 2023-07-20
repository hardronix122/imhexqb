#ifndef QBCRC_H
#define QBCRC_H

#include <string>
#include <algorithm>

class qb_crc {
public:
    static unsigned long generate(std::string text);
};


#endif
