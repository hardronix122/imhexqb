#ifndef QBCRC_H
#define QBCRC_H

#include <string>
#include <algorithm>

class QbCrc {
public:
    static unsigned long generate(std::string text);
};


#endif
