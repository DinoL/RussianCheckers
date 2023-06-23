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
        const char sep = p.size() > 2 ? 'x' : '-';
        s << i+1 << ". ";
        for (int j = 0; j < p.size(); ++j)
        {
            s << p[j] + 1;
            if (j != p.size() - 1)
            {
                s << sep;
            }
        }
        if (i != data._moves.size() -1)
        {
            s << ' ';
        }
    }
}
