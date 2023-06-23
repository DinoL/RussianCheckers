#ifndef PDNPARSER_H
#define PDNPARSER_H

#include "pdn.h"

#include <iostream>

class PdnParser
{
public:
    PdnParser();

    static PDN read(std::istream& s);
    static void write(const PDN& data, std::ostream& s);

};

#endif // PDNPARSER_H
