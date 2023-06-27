#include "pdnparser.h"

PDN PdnParser::read(std::istream& s, CellFormat format)
{
    PDN pdn;

    std::string cur_turn;
    std::string white_move_str;
    std::string black_move_str;

    while (s)
    {
        s >> cur_turn >> white_move_str >> black_move_str;

        const PDN::Move white_move = move_from_string(white_move_str, format);
        const PDN::Move black_move = move_from_string(black_move_str, format);
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

void PdnParser::write(const PDN& data, std::ostream& s, CellFormat format)
{
    const auto& moves = data._moves;
    for (int i = 0; i < moves.size(); ++i)
    {
        if (i % 2 == 0)
        {
            s << i/2 + 1 << ". ";
        }

        s << move_to_string(moves[i], format);

        if (i != moves.size() - 1)
        {
            s << ' ';
        }
    }
}

std::string PdnParser::cell_to_string(int cell, CellFormat format)
{
    switch(format)
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

int PdnParser::cell_from_string(const std::string& s, CellFormat format)
{
    switch(format)
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

std::string PdnParser::move_to_string(const PDN::Move& move, CellFormat format)
{
    std::string result;
    const auto& cells = move._cells;
    const char sep = move._is_eat ? 'x' : '-';
    for (int i = 0; i < cells.size(); ++i)
    {
        result += cell_to_string(cells[i], format);
        if (i != cells.size() - 1)
        {
            result += sep;
        }
    }
    return result;
}

PDN::Move PdnParser::move_from_string(const std::string& s, CellFormat format)
{
    const bool is_eat = (s.find('x') != std::string::npos);

    int cells_cnt = (s.size() / 3 + 1);
    std::vector<int> cells;
    for (int i = 0; i < cells_cnt; ++i)
    {
        cells.push_back(PdnParser::cell_from_string(s.substr(3*i, 2), format));
    }
    return PDN::Move{cells, is_eat};
}
