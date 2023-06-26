#ifndef PDNPARSER_H
#define PDNPARSER_H

#include "pdn.h"

#include <iostream>
#include <string>

enum class CellFormat
{
    NUMERIC,
    ALPHANUMERIC
};

class PdnParser
{
public:
    PdnParser();

    static PDN read(std::istream& s, CellFormat format);
    static void write(const PDN& data, std::ostream& s,
                      CellFormat format = CellFormat::NUMERIC);
    static std::string format_cell(int cell, CellFormat format);
    static int cell_from_string(const std::string& s, CellFormat format);

};

#endif // PDNPARSER_H
