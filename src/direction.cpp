#include "direction.h"

#include <cmath>

Direction::Direction(int even_step, int odd_step, state_t border)
    : _even_step(even_step)
    , _odd_step(odd_step)
    , _border(border)
{}

bool Direction::up() const
{
    return _even_step > 0 && _odd_step > 0;
}

int Direction::step(state_t s) const
{
    return (s & constants::EVEN) ? std::abs(_even_step) : std::abs(_odd_step);
}

state_t Direction::move(state_t s) const
{
    return up() ? s << step(s) : s >> step(s);
}

state_t Direction::moves(state_t s) const
{
    state_t res = 0;
    state_t next = move(s);
    while (s & ~_border)
    {
        res |= next;
        s = next;
        next = move(s);
    }
    return res;
}

Direction Direction::top_right()
{
    return Direction(4, 5, constants::TOP | constants::RIGHT);
}

Direction Direction::top_left()
{
    return Direction(3, 4, constants::TOP | constants::LEFT);
}

Direction Direction::bottom_right()
{
    return Direction(-4, -3, constants::BOTTOM | constants::RIGHT);
}

Direction Direction::bottom_left()
{
    return Direction(-5, -4, constants::BOTTOM | constants::LEFT);
}
