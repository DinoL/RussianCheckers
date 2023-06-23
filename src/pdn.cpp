#include "pdn.h"

#include "bit_algo.h"
#include "checkerslogic.h"

PDN::PDN()
{}

PDN PDN::from_history(const History& hist)
{
    PDN res;
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

        std::vector<int> move_cells;
        move_cells.push_back(cell_from);

        state_t between = (CheckersLogic::get_between(state_from, state_to)
                          |CheckersLogic::get_between(state_to, state_from));
        while (between)
        {
            const int cell = alg::first_set_cell(between);
            between ^= alg::to_state(cell);
            move_cells.push_back(cell);
        }
        move_cells.push_back(cell_to);

        res._moves.push_back(move_cells);
    }
    return res;
}
