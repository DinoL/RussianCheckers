#include "direction.h"

#include "bit_algo.h"

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

state_t Direction::free_moves(state_t s, state_t p) const
{
    state_t res = 0;

    while (s)
    {
        state_t cur = alg::first_set_piece(s);
        s ^= cur;

        state_t next = move(cur);
        while ((next & ~p) && (cur & ~_border))
        {
            res |= next;
            cur = next;
            next = move(cur);
        }
    }
    return res;
}

state_t Direction::eat_moves(state_t s, state_t b, state_t p) const
{
    state_t res = 0;

    while (s)
    {
        state_t cur = alg::first_set_piece(s);
        s ^= cur;

        state_t next = move(cur);
        while ((next & ~p) && (cur & ~_border))
        {
            cur = next;
            next = move(cur);
        }
        if ((cur & ~_border) && (next & b))
        {
            cur = next;
            next = move(cur);
            while ((cur & ~_border) && (next & ~p))
            {
                res |= next;
                cur = next;
                next = move(cur);
            }
        }
    }

    return res;
}

state_t Direction::all_moves(state_t s) const
{
    state_t res = 0;

    while (s)
    {
        state_t cur = alg::first_set_piece(s);
        s ^= cur;

        state_t next = move(cur);
        while (cur & ~_border)
        {
            res |= next;
            cur = next;
            next = move(cur);
        }
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
