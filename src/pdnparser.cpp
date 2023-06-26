#include "pdnparser.h"

PdnParser::PdnParser()
{}

PDN PdnParser::read(std::istream& s)
{
    PDN pdn;

    int cur_turn;
    char dot;
    char sep;

    int from;
    int to;

    while (s)
    {
        s >> cur_turn >> dot >> from >> sep >> to;

        const bool is_eat = (sep == 'x');
        std::vector<int> cells = {from - 1, to - 1};

        while (s && s.peek() != ' ')
        {
            s >> sep >> to;
            cells.push_back(to - 1);
        }

        pdn._moves.push_back(PDN::Move{cells, is_eat});
    }

    return pdn;
}

void PdnParser::write(const PDN& data, std::ostream& s)
{
    for (int i = 0; i < data._moves.size(); ++i)
    {
        const auto& p = data._moves[i];
        const char sep = p._is_eat ? 'x' : '-';
        s << i+1 << ". ";

        for (int j = 0; j < p._cells.size(); ++j)
        {
            s << p._cells[j] + 1;
            if (j != p._cells.size() - 1)
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
