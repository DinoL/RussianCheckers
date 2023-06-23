#include "pdnparser.h"

PdnParser::PdnParser()
{}

PDN PdnParser::read(std::istream& s)
{
    return PDN();
}

void PdnParser::write(const PDN& data, std::ostream& s)
{
    for (const auto& p : data._moves)
    {
        s << p.first << ' ' << p.second << std::endl;
    }
}
