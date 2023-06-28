#include "pdnparser.h"

#include <sstream>
#include <string>

PDN PdnParser::read(std::istream& s) const
{
    const MoveParser mp(_format);
    PDN pdn;

    std::string cur_turn;
    std::string white_move_str;
    std::string black_move_str;

    while (s.good())
    {
        s >> cur_turn >> white_move_str >> black_move_str;

        const PDN::Move white_move = mp.decode(white_move_str);
        const PDN::Move black_move = mp.decode(black_move_str);
        if (white_move.valid())
        {
            pdn._moves.push_back(white_move);
        }
        if (black_move.valid())
        {
            pdn._moves.push_back(black_move);
        }
    }

    return pdn;
}

void PdnParser::write(const PDN& data, std::ostream& s) const
{
    const MoveParser mp(_format);
    const auto& moves = data._moves;
    for (int i = 0; i < moves.size(); ++i)
    {
        if (i % 2 == 0)
        {
            s << i/2 + 1 << _turns_sep << ' ';
        }

        s << mp.encode(moves[i]);

        if (i != moves.size() - 1)
        {
            s << ' ';
        }
    }
}

std::string CellParser::encode(int cell) const
{
    switch(_format)
    {
    case CellFormat::NUMERIC:
        return std::to_string(cell + 1);
    case CellFormat::ALPHANUMERIC:
        const bool even_row = ((cell / 4) % 2 == 0);
        char col = even_row ? "aceg"[cell % 4]
                            : "bdfh"[cell % 4];
        return col + std::to_string(cell / 4 + 1);
    }
    return "";
}

int CellParser::decode(const std::string& s) const
{
    switch(_format)
    {
    case CellFormat::NUMERIC:
        return std::stoi(s) - 1;
    case CellFormat::ALPHANUMERIC:
        const char col_id = s[0];
        const int row = s[1] - '1';
        const bool even_row = (row % 2 == 0);
        const int col = even_row ? (col_id - 'a') / 2
                                 : (col_id - 'a' - 1) / 2;
        return row * 4 + col;
    }
    return -1;
}

std::string MoveParser::encode(const PDN::Move& move) const
{
    const CellParser cp(_format);
    std::string result;
    const auto& cells = move._cells;
    const char sep = move._is_eat ? _eat_sep : _step_sep;
    for (int i = 0; i < cells.size(); ++i)
    {
        result += cp.encode(cells[i]);
        if (i != cells.size() - 1)
        {
            result += sep;
        }
    }
    return result;
}

PDN::Move MoveParser::decode(const std::string& s) const
{
    const CellParser cp(_format);
    const bool is_eat = (s.find(_eat_sep) != std::string::npos);
    const char sep = is_eat ? _eat_sep : _step_sep;

    std::stringstream ss(s);
    std::string item;

    std::vector<int> cells;
    while(std::getline(ss, item, sep))
    {
        cells.push_back(cp.decode(item));
    }

    return PDN::Move{cells, is_eat};
}
