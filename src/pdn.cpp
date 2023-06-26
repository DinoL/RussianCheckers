#include "pdn.h"

#include "bit_algo.h"
#include "checkerslogic.h"

PDN::PDN()
{}

PDN PDN::from_history(const History& hist)
{
    PDN res;

    PDN::Move cur_move;
    for (int i = 1; i < hist.size(); ++i)
    {
        const BoardState& prev = hist[i-1];
        const BoardState& cur = hist[i];
        const bool is_white = prev._white_turn;
        const state_t from = prev.get_state(is_white);
        const state_t to = cur.get_state(is_white);
        const state_t state_from = (from & (from ^ to));
        const state_t state_to = (to & (from ^ to));
        const int cell_from = alg::first_set_cell(state_from);
        const int cell_to = alg::first_set_cell(state_to);

        const state_t between = (CheckersLogic::get_between(state_from, state_to)
                          |      CheckersLogic::get_between(state_to, state_from));
        if (cur_move._cells.empty())
        {
            cur_move._cells.push_back(cell_from);
        }
        cur_move._cells.push_back(cell_to);
        cur_move._is_eat = (between != 0);

        if (prev._white_turn != cur._white_turn)
        {
            res._moves.push_back(cur_move);
            cur_move = PDN::Move();
        }
    }
    if (!cur_move._cells.empty())
    {
        res._moves.push_back(cur_move);
    }

    return res;
}

History PDN::to_history() const
{
    History history;
    CheckersLogic logic;

    history.push(logic.state());

    for (const PDN::Move& move : _moves)
    {
        for (int i = 1; i < move._cells.size(); ++i)
        {
            logic.move_piece(move._cells[i-1], move._cells[i]);
            history.push(logic.state());
        }
    }
    return history;
}

bool PDN::Move::operator==(const Move& other) const
{
    return _cells == other._cells
           && _is_eat == other._is_eat;
}

bool PDN::Move::valid() const
{
    if (_cells.size() < 2)
    {
        return false;
    }
    if (_cells.size() > 2 && !_is_eat)
    {
        return false;
    }
    for (int c : _cells)
    {
        if (c < 0 || c > 31)
        {
            return false;
        }
    }

    return true;
}
