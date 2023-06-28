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

class CellParser
{
public:
    CellParser(CellFormat format) : _format(format)
    {}

    std::string encode(int cell) const;
    int decode(const std::string& s) const;

private:
    CellFormat _format;
};

class MoveParser
{
public:
    MoveParser(CellFormat format) : _format(format)
    {}

    std::string encode(const PDN::Move& move) const;
    PDN::Move decode(const std::string& s) const;

private:
    CellFormat _format;
    const char _eat_sep = 'x';
    const char _step_sep = '-';
};

class PdnParser
{
public:
    PdnParser(CellFormat format) : _format(format)
    {}

    PDN read(std::istream& s) const;
    void write(const PDN& data, std::ostream& s) const;

private:
    CellFormat _format;
    const char _turns_sep = '.';
};

#endif // PDNPARSER_H
