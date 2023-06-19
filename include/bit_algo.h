#ifndef BIT_ALGO_H
#define BIT_ALGO_H

#include "defines.h"

namespace alg
{

    bool has_piece(state_t s, int cell);
    state_t remove_piece(state_t s, int cell);
    state_t set_piece(state_t s, int cell);
    state_t to_state(int cell);
    state_t last_set_piece(state_t s);

}

#endif // BIT_ALGO_H
