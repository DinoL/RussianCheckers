#include "checkerslogic.h"
#include "bit_algo.h"

CheckersLogic::CheckersLogic(const BoardState& state /*=BoardState()*/)
    : _state(state)
{}

void CheckersLogic::switch_turn()
{
    _state.switch_turn();
}

bool CheckersLogic::is_white_turn() const
{
    return _state._white_turn;
}

int CheckersLogic::eating_piece() const
{
    return _state._eating_piece;
}

BoardState CheckersLogic::state() const
{
    return _state;
}

state_t CheckersLogic::get_state(bool is_white) const
{
    return _state.get_state(is_white);
}

state_t CheckersLogic::get_kings_state(bool is_white) const
{
    return _state.get_kings_state(is_white);
}

state_t CheckersLogic::step_moves(state_t s, bool is_white) const
{
    const state_t p = filled();
    const state_t kings = get_kings_state(is_white);
    const state_t kings_moves = king_step_moves(s & kings, p);
    const state_t men_moves = man_step_moves(s & ~kings, is_white);
    return ~p & (men_moves | kings_moves);
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
        (((s & 0x70707 & (b >> 4)) << 9)
        |((s & 0x707070 & (b >> 5)) << 9)
        |((s & 0xe0e0e0 & (b >> 4)) << 7)
        |((s & 0xe0e0e & (b >> 3)) << 7)
        |((s & 0xe0e0e000 & (b << 4)) >> 9)
        |((s & 0xe0e0e00 & (b << 5)) >> 9)
        |((s & 0x7070700 & (b << 4)) >> 7)
        |((s & 0x70707000 & (b << 3)) >> 7));
}

state_t CheckersLogic::eat_moves(state_t s, bool is_white) const
{
    const state_t p = filled();
    const state_t b = get_state(!is_white);
    const state_t kings = get_kings_state(is_white);
    const state_t kings_eat_moves = king_eat_moves(s & kings, b, p);
    const state_t men_eat_moves = man_eat_moves(s & ~kings, b);
    return ~p & (men_eat_moves | kings_eat_moves);
}

state_t CheckersLogic::king_step_moves(state_t s, state_t p)
{
    return Direction::top_right().free_moves(s, p)
         | Direction::bottom_left().free_moves(s, p)
         | Direction::top_left().free_moves(s, p)
         | Direction::bottom_right().free_moves(s, p);
}

state_t CheckersLogic::king_eat_moves_in_direction(state_t s, state_t b, state_t p, const Direction& dir)
{
    state_t res = 0;

    while (s)
    {
        const state_t start = alg::first_set_piece(s);
        s ^= start;

        const state_t moves = dir.eat_moves(start, b, p);
        state_t moves_to_check = moves;
        state_t continuous_eat_moves = 0;

        while (moves_to_check)
        {
            state_t end = alg::first_set_piece(moves_to_check);
            moves_to_check ^= end;

            state_t b2 = b;
            state_t to_remove = (get_between(start, end)
                               | get_between(end, start));
            b2 &= ~to_remove;

            state_t p2 = p;
            p2 &= ~to_remove;
            p2 &= ~start;
            p2 |= end;

            if (king_eat_moves_unfiltered(end, b2, p2))
            {
                continuous_eat_moves |= end;
            }
        }

        res |= (continuous_eat_moves ? continuous_eat_moves : moves);
    }

    return res;
}

state_t CheckersLogic::king_eat_moves_unfiltered(state_t s, state_t b, state_t p)
{
    return Direction::top_right().eat_moves(s, b, p)
         | Direction::bottom_left().eat_moves(s, b, p)
         | Direction::top_left().eat_moves(s, b, p)
         | Direction::bottom_right().eat_moves(s, b, p);
}

state_t CheckersLogic::king_eat_moves(state_t s, state_t b, state_t p)
{
    state_t moves = 0;

    while (s)
    {
        state_t start = alg::first_set_piece(s);
        s ^= start;

        for (const auto& dir : {Direction::top_right(),
                                Direction::top_left(),
                                Direction::bottom_right(),
                                Direction::bottom_left()})
        {
            moves |= king_eat_moves_in_direction(start, b, p, dir);
        }
    }

    return moves;
}

state_t CheckersLogic::moves(state_t s, bool is_white) const
{
    if (_state._eating_piece >= 0)
    {
        return eat_moves(s & alg::to_state(_state._eating_piece), is_white);
    }
    return current_eat_moves() ?
               eat_moves(s, is_white) :
               step_moves(s, is_white);
}

state_t CheckersLogic::current_moves() const
{
    return moves(get_state(is_white_turn()), is_white_turn());
}

state_t CheckersLogic::current_eat_moves() const
{
    return eat_moves(get_state(is_white_turn()), is_white_turn());
}

void CheckersLogic::move_piece(int piece, int cell)
{
    const bool eat_move = current_eat_moves();

    _state.move_piece(piece, cell);

    if (eat_move)
    {
        const state_t start = alg::to_state(std::min(piece, cell));
        const state_t end = alg::to_state(std::max(piece, cell));
        const state_t to_remove = get_between(start, end);
        eat_from_cells(to_remove);
    }

    const bool can_eat_more = eat_moves(alg::to_state(cell), is_white_turn());
    if (eat_move && can_eat_more)
    {
        _state._eating_piece = cell;
    }
    else
    {
        switch_turn();
        _state._eating_piece = -1;
        _state._eaten = 0;
    }
}

void CheckersLogic::eat_from_cells(state_t to_remove)
{
    _state.eat_from_cells(to_remove);
}

void CheckersLogic::set_state(const BoardState& state)
{
    _state = state;
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
    return _state.filled();
}

void CheckersLogic::reset()
{
    _state = BoardState();
}

