#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include "constants.h"
#include "bit_algo.h"

class BoardState
{
public:
    state_t _white = constants::WHITE_START;
    state_t _black = constants::BLACK_START;
    state_t _white_kings = 0;
    state_t _black_kings = 0;
    bool _white_turn = true;
    int _eating_piece = -1;

    bool operator==(const BoardState& other) const;

    state_t filled() const
    {
        return _white | _black;
    }

    void switch_turn()
    {
        _white_turn = !_white_turn;
    }

    void clear_cells(state_t to_remove);

    state_t get_state(bool is_white) const
    {
        return is_white ? _white : _black;
    }

    state_t get_kings_state(bool is_white) const
    {
        return is_white ? _white_kings : _black_kings;
    }

    void move_piece(int piece, int cell);
};

#endif // BOARDSTATE_H
