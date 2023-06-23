#include "pdn.h"

#include "bit_algo.h"

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

        const int cell_from = alg::first_set_cell(from & (from ^ to));
        const int cell_to = alg::first_set_cell(to & (from ^ to));

        res._moves.push_back({cell_from + 1, cell_to + 1});
    }
    return res;
}
