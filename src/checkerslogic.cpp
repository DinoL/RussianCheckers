#include "checkerslogic.h"
#include "bit_algo.h"

CheckersLogic::CheckersLogic()
{
    reset();
}

void CheckersLogic::switch_turn()
{
    _white_turn = !_white_turn;
}

bool CheckersLogic::is_white_turn() const
{
    return _white_turn;
}

state_t CheckersLogic::get_state(bool is_white) const
{
    return is_white ? _white : _black;
}

state_t CheckersLogic::get_kings_state(bool is_white) const
{
    return is_white ? _white_kings : _black_kings;
}

state_t CheckersLogic::step_moves(state_t s, bool is_white) const
{
    const state_t man_moves = man_step_moves(s, is_white);
    const state_t king_moves = king_step_moves(s & get_kings_state(is_white));
    return ~filled() & (man_moves | king_moves);
}

state_t CheckersLogic::man_step_moves(state_t s, bool is_white)
{
    return is_white ?
               (s << 4) | ((s & 0xe0e0e0e) << 3) | ((s & 0x707070) << 5) :
               (s >> 4) | ((s & 0xe0e0e0e) >> 5) | ((s & 0x70707070) >> 3);
}

state_t CheckersLogic::man_eat_moves(state_t s, state_t b)
{
    return
        (((s  & 0x70707 & (b >> 4)) << 9)
        |((s  & 0x707070 & (b >> 5)) << 9)
        |((s  & 0xe0e0e0 & (b >> 4)) << 7)
        |((s  & 0xe0e0e & (b >> 3)) << 7)
        |((s  & 0xe0e0e000 & (b << 4)) >> 9)
        |((s  & 0xe0e0e00 & (b << 5)) >> 9)
        |((s  & 0x7070700 & (b << 4)) >> 7)
        |((s  & 0x70707000 & (b << 3)) >> 7));
}

state_t CheckersLogic::eat_moves(state_t s, bool is_white) const
{
    const state_t b = get_state(!is_white);
    const state_t kings = get_kings_state(is_white);
    const state_t kings_eat_moves = king_eat_moves(s & kings, b, filled());
    const state_t men_eat_moves = man_eat_moves(s & ~kings, b);
    return ~filled() & (men_eat_moves | kings_eat_moves);
}

state_t CheckersLogic::king_step_moves(state_t s) const
{
    state_t p = filled();

    return Direction::top_right().free_moves(s, p)
         | Direction::bottom_left().free_moves(s, p)
         | Direction::top_left().free_moves(s, p)
         | Direction::bottom_right().free_moves(s, p);
}

state_t CheckersLogic::king_eat_moves_in_direction(state_t s, state_t opponent, state_t filled, const Direction& dir) const
{
    const state_t p = filled;
    const state_t start = s;

    state_t moves = 0;
    state_t next = dir.move(s);
    while ((next & ~p) && (s & ~dir._border))
    {
        s = next;
        next = dir.move(s);
    }
    if ((s & ~dir._border) && (next & opponent))
    {
        s = next;
        next = dir.move(s);
        while ((next & ~p) && (s & ~dir._border))
        {
            moves |= next;
            s = next;
            next = dir.move(s);
        }
    }

    state_t continuous_eat_moves = 0;
    state_t moves_to_check = moves;
    while (moves_to_check)
    {
        state_t end = alg::first_set_piece(moves_to_check);
        moves_to_check ^= end;

        state_t opp = opponent;
        state_t to_remove = (get_between(start, end) | get_between(end, start));
        opp &= ~to_remove;

        state_t filled_after_move = p;
        filled_after_move &= ~to_remove;
        filled_after_move &= ~start;
        filled_after_move |= end;

        bool can_continue = (king_eat_moves(end, opp, filled_after_move) != 0);
        if (can_continue)
        {
            continuous_eat_moves |= end;
        }
    }

    return continuous_eat_moves ? continuous_eat_moves : moves;
}

state_t CheckersLogic::king_eat_moves(state_t s, state_t opponent, state_t filled) const
{
    state_t moves = 0;

    while (s)
    {
        state_t start = alg::first_set_piece(s);
        s ^= start;

        moves |= king_eat_moves_in_direction(start, opponent, filled, Direction::top_right());
        moves |= king_eat_moves_in_direction(start, opponent, filled, Direction::bottom_left());
        moves |= king_eat_moves_in_direction(start, opponent, filled, Direction::top_left());
        moves |= king_eat_moves_in_direction(start, opponent, filled, Direction::bottom_right());
    }

    return moves;
}

state_t CheckersLogic::moves(state_t s, bool is_white) const
{
    if (_eatingPiece >= 0)
    {
        return eat_moves(s & alg::to_state(_eatingPiece), is_white);
    }
    return current_eat_moves() ?
               eat_moves(s, is_white) :
               step_moves(s, is_white);
}

state_t CheckersLogic::current_moves() const
{
    return moves(get_state(_white_turn), _white_turn);
}

state_t CheckersLogic::current_eat_moves() const
{
    return eat_moves(get_state(_white_turn), _white_turn);
}

void CheckersLogic::move_piece(int piece, int cell)
{
    const bool eat_move = current_eat_moves();

    state_t a = get_state(_white_turn);
    a = alg::remove_piece(a, piece);
    a = alg::set_piece(a, cell);

    state_t& kings = _white_turn ? _white_kings : _black_kings;
    if (alg::has_piece(kings, piece))
    {
        kings = alg::remove_piece(kings, piece);
        kings = alg::set_piece(kings, cell);
    }

    bool become_king = (_white_turn && cell > 27) || (!_white_turn && cell < 4);
    if (become_king)
    {
        kings = alg::set_piece(kings, cell);
    }

    if (_white_turn)
        _white = a;
    else
        _black = a;

    if (eat_move)
    {
        const state_t start = alg::to_state(std::min(piece, cell));
        const state_t end = alg::to_state(std::max(piece, cell));
        const state_t to_remove = get_between(start, end);
        clear_cells(to_remove);
    }

    bool can_eat_more = eat_moves(alg::to_state(cell), _white_turn);
    if (eat_move && can_eat_more)
    {
        _activePiece = cell;
        _eatingPiece = cell;
    }
    else
    {
        switch_turn();
        _activePiece = -1;
        _eatingPiece = -1;
    }
}

void CheckersLogic::clear_cells(state_t to_remove)
{
    state_t negate = ~to_remove;
    _white &= negate;
    _white_kings &= negate;
    _black &= negate;
    _black_kings &= negate;
}

state_t CheckersLogic::get_between(state_t start, state_t end)
{
    return ~(start | end) &
            (Direction::top_right().all_moves(start) &
             Direction::bottom_left().all_moves(end)) |
            (Direction::top_left().all_moves(start) &
             Direction::bottom_right().all_moves(end));
}

state_t CheckersLogic::filled() const
{
    return _white | _black;
}

void CheckersLogic::reset()
{
    _white = constants::WHITE_START;
    _black = constants::BLACK_START;
    _white_kings = 0;
    _black_kings = 0;
    _white_turn = true;
    _eatingPiece = -1;
    _activePiece = -1;
}

int CheckersLogic::active_piece() const
{
    return _activePiece;
}
