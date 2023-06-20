#ifndef DIRECTION_H
#define DIRECTION_H

#include "constants.h"

class Direction
{
public:
    Direction(int even_step, int odd_step, state_t border);

    bool up() const;
    int step(state_t s) const;
    state_t move(state_t s) const;
    state_t all_moves(state_t s) const;
    state_t free_moves(state_t s, state_t p) const;

    static Direction top_right();
    static Direction top_left();
    static Direction bottom_right();
    static Direction bottom_left();

    const state_t _border;
    const int _even_step;
    const int _odd_step;
};

#endif // DIRECTION_H
