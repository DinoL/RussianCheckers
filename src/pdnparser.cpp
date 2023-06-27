#include "pdnparser.h"

#include <iostream>

PdnParser::PdnParser()
{}

PDN read_numeric(std::istream& s)
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
            if (s)
            {
                cells.push_back(to - 1);
            }
        }

        const PDN::Move move{cells, is_eat};
        if (move.valid())
        {
            pdn._moves.push_back(move);
        }

        if (!s)
        {
            continue;
        }

        s >> from >> sep >> to;

        const bool is_eat2 = (sep == 'x');
        cells = {from - 1, to - 1};

        while (s && s.peek() != ' ')
        {
            s >> sep >> to;
            if (s)
            {
                cells.push_back(to - 1);
            }
        }

        const PDN::Move move2{cells, is_eat2};
        if (move2.valid())
        {
            pdn._moves.push_back(move2);
        }
    }

    return pdn;
}

PDN::Move move_from_string(const std::string& s)
{
    const bool is_eat = (s.find('x') != std::string::npos);

    int cells_cnt = (s.size() / 3 + 1);
    std::vector<int> cells;
    for (int i = 0; i < cells_cnt; ++i)
    {
        cells.push_back(PdnParser::cell_from_string(s.substr(3*i, 2), CellFormat::ALPHANUMERIC));
    }
    return PDN::Move{cells, is_eat};
}

PDN read_alphanumeric(std::istream& s)
{
    PDN pdn;

    std::string cur_turn;
    std::string white_move_str;
    std::string black_move_str;

    while (s)
    {
        s >> cur_turn >> white_move_str >> black_move_str;

        const PDN::Move white_move = move_from_string(white_move_str);
        const PDN::Move black_move = move_from_string(black_move_str);
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

PDN PdnParser::read(std::istream& s, CellFormat format)
{
    switch(format)
    {
    case CellFormat::NUMERIC:
        return read_numeric(s);
    case CellFormat::ALPHANUMERIC:
        return read_alphanumeric(s);
    }
    return PDN();
}

void PdnParser::write(const PDN& data, std::ostream& s,
                      CellFormat format /*= CellFormat::NUMERIC*/)
{
    for (int i = 0; i < data._moves.size(); ++i)
    {
        const auto& p = data._moves[i];
        const char sep = p._is_eat ? 'x' : '-';
        if (i % 2 == 0)
        {
            s << i/2 + 1 << ". ";
        }
        for (int j = 0; j < p._cells.size(); ++j)
        {
            s << format_cell(p._cells[j], format);
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

std::string PdnParser::format_cell(int cell, CellFormat format)
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

