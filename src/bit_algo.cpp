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

}
