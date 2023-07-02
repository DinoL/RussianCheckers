#include "boardstate.h"

bool BoardState::operator==(const BoardState& other) const
{
    return _white == other._white
        && _black == other._black
        && _white_kings == other._white_kings
        && _black_kings == other._black_kings
        && _eaten == other._eaten
        && _white_turn == other._white_turn
        && _eating_piece == other._eating_piece;
}

bool BoardState::valid() const
{
    return ((_white & _white_kings) == _white_kings)
        && ((_white | _white_kings) == _white)
        && ((_black & _black_kings) == _black_kings)
        && ((_black | _black_kings) == _black)
        && ((_white & _black) == 0)
        && ((_eaten & _white) == 0)
        && ((_eaten & _black) == 0)
        && ((_white_kings & _black_kings) == 0)
        && (_eating_piece >= -1)
        && (_eating_piece < 32)
        && ((_eating_piece < 0) || ((_white | _black) & alg::to_state(_eating_piece)))
        && (((_eating_piece >= 0) && (_white & alg::to_state(_eating_piece)))
        == ((_eating_piece >= 0) && _white_turn));
}

void BoardState::clear_cells(state_t to_remove)
{
    state_t negate = ~to_remove;
    _white &= negate;
    _white_kings &= negate;
    _black &= negate;
    _black_kings &= negate;
}

void BoardState::move_piece(int piece, int cell)
{
    const bool is_white_piece = (_white & alg::to_state(piece));

    _white = alg::move_piece(_white, piece, cell);
    _black = alg::move_piece(_black, piece, cell);
    _white_kings = alg::move_piece(_white_kings, piece, cell);
    _black_kings = alg::move_piece(_black_kings, piece, cell);

    if (is_white_piece && cell > 27)
    {
        _white_kings = alg::set_piece(_white_kings, cell);
    }
    if (!is_white_piece && cell < 4)
    {
        _black_kings = alg::set_piece(_black_kings, cell);
    }
}
