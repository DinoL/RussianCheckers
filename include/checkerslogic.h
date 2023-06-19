#ifndef CHECKERSLOGIC_H
#define CHECKERSLOGIC_H

#include <cstdint>
#include <algorithm>

#include "defines.h"
#include "direction.h"

class CheckersLogic
{
public:
    CheckersLogic();

    // modifiers
    void switch_turn();
    void reset();
    void move_piece(int piece, int cell);

    state_t step_moves(state_t s, bool is_white) const;
    state_t eat_moves(state_t s, bool is_white) const;
    state_t king_moves_in_direction(state_t s, const Direction& dir) const;
    state_t king_moves(state_t s) const;
    state_t king_eat_moves_in_direction(state_t s, state_t opponent, state_t filled, const Direction& dir) const;
    bool king_has_eat_moves(state_t s, state_t opponent, state_t filled) const;
    state_t king_eat_moves(state_t s, state_t opponent) const;
    state_t moves(state_t s, bool is_white) const;
    state_t current_moves() const;
    state_t current_eat_moves() const;
    bool has_piece(state_t s, int cell) const;
    state_t remove_piece(state_t s, int cell) const;
    state_t set_piece(state_t s, int cell) const;
    state_t filled() const;

    // getters
    int active_piece() const;
    state_t get_state(bool is_white) const;
    state_t get_kings_state(bool is_white) const;
    bool is_white_turn() const;

    // static
    static state_t get_between(state_t start, state_t end);
    static state_t straight_moves_in_direction(state_t s, const Direction& dir);

private:
    state_t _white;
    state_t _black;
    state_t _white_kings;
    state_t _black_kings;
    bool _white_turn;
    int _eatingPiece;
    int _activePiece;
};

#endif // CHECKERSLOGIC_H
