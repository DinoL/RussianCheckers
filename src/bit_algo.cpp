#include "bit_algo.h"

namespace alg
{

    bool has_piece(state_t s, int cell)
    {
        return s & to_state(cell);
    }

    state_t remove_piece(state_t s, int cell)
    {
        return s & ~to_state(cell);
    }

    state_t set_piece(state_t s, int cell)
    {
        return s | to_state(cell);
    }

    state_t to_state(int cell)
    {
        return 1 << cell;
    }

    state_t first_set_piece(state_t s)
    {
        return s & ~(s-1);
    }

    state_t add(state_t s, state_t r)
    {
        return s | r;
    }

    state_t subtract(state_t s, state_t r)
    {
        return s & ~r;
    }

    state_t move_piece(state_t s, int from, int to)
    {
        state_t res = s;
        if (s & alg::to_state(from))
        {
            res = alg::remove_piece(res, from);
            res = alg::set_piece(res, to);
        }
        return res;
    }

    int first_set_cell(state_t s)
    {
        if (s == 0)
            return -1;

        state_t c = first_set_piece(s);
        int k = 0;
        while (c && (c % 2 == 0))
        {
            ++k;
            c = c >> 1;
        }
        return k;
    }

}
