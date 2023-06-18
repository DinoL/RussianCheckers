#ifndef DIRECTION_H
#define DIRECTION_H

#include <cstdint>
#include <cmath>

using state_t = uint32_t;

class Direction
{
public:
    const state_t _even = 0xf0f0f0f;
    const state_t _border;
    const int _even_step;
    const int _odd_step;

    Direction(int es, int os, state_t border)
        : _even_step(es)
        , _odd_step(os)
        , _border(border)
    {}

    bool up() const
    {
        return _even_step > 0 && _odd_step > 0;
    }

    int step(state_t s) const
    {
        return (s & _even) ? std::abs(_even_step) : std::abs(_odd_step);
    }

    state_t move(state_t s) const
    {
        return up() ? s << step(s) : s >> step(s);
    }

    state_t moves(state_t s) const
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
};

#endif // DIRECTION_H
