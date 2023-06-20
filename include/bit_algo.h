#ifndef BIT_ALGO_H
#define BIT_ALGO_H

#include "defines.h"

namespace alg
{

    bool has_piece(state_t s, int cell);
    state_t remove_piece(state_t s, int cell);
    state_t set_piece(state_t s, int cell);
    state_t to_state(int cell);
    state_t first_set_piece(state_t s);
    state_t add(state_t s, state_t r);
    state_t subtract(state_t s, state_t r);
    state_t move_piece(state_t s, int from, int to);

}

#endif // BIT_ALGO_H
