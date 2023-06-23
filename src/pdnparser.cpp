#include "pdnparser.h"

PdnParser::PdnParser()
{}

PDN PdnParser::read(std::istream& s)
{
    return PDN();
}

void PdnParser::write(const PDN& data, std::ostream& s)
{
    for (int i = 0; i < data._moves.size(); ++i)
    {
        const auto& p = data._moves[i];
        s << i+1 << ". " << p.first << '-' << p.second;
        if (i != data._moves.size() -1)
        {
            s << ' ';
        }
    }
}
