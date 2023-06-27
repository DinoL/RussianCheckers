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
    static PDN read(std::istream& s, CellFormat format);
    static void write(const PDN& data, std::ostream& s, CellFormat format);
    static std::string cell_to_string(int cell, CellFormat format);
    static int cell_from_string(const std::string& s, CellFormat format);
    static std::string move_to_string(const PDN::Move& move, CellFormat format);
    static PDN::Move move_from_string(const std::string& s, CellFormat format);
};

#endif // PDNPARSER_H
