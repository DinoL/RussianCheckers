#ifndef CHECKERSLOGIC_H
#define CHECKERSLOGIC_H

#include <cstdint>
#include <algorithm>

#include "defines.h"
#include "direction.h"
#include "boardstate.h"

class CheckersLogic
{
public:
    CheckersLogic(const BoardState& state = BoardState());

    // modifiers
    void switch_turn();
    void reset();
    void move_piece(int piece, int cell);
    void clear_cells(state_t to_remove);

    // queries
    state_t moves(state_t s, bool is_white) const;
    state_t step_moves(state_t s, bool is_white) const;
    state_t eat_moves(state_t s, bool is_white) const;
    state_t current_moves() const;
    state_t current_eat_moves() const;
    state_t filled() const;

    // getters
    state_t get_state(bool is_white) const;
    state_t get_kings_state(bool is_white) const;
    bool is_white_turn() const;
    int eating_piece() const;

    // static
    static state_t get_between(state_t start, state_t end);
    static state_t man_step_moves(state_t s, bool is_white);
    static state_t man_eat_moves(state_t s, state_t b);
    static state_t king_step_moves(state_t s, state_t p );
    static state_t king_eat_moves_unfiltered(state_t s, state_t b, state_t p);
    static state_t king_eat_moves_in_direction(state_t s, state_t b, state_t p, const Direction& dir);
    static state_t king_eat_moves(state_t s, state_t b, state_t p);

private:
    BoardState _state;
};

#endif // CHECKERSLOGIC_H
