#ifndef MODEL_H
#define MODEL_H

#include <QObject>

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
        return (s & _even) ? abs(_even_step) : abs(_odd_step);
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


class MyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int activePiece READ activePiece WRITE setActivePiece NOTIFY onActivePieceChange)
    Q_PROPERTY(bool whiteTurn READ whiteTurn WRITE setWhiteTurn NOTIFY onTurnChange)
public:
    using state_t = uint32_t;

    MyModel(QObject* parent = nullptr)
    {
        Q_UNUSED(parent);
        reset();
    }

    Q_INVOKABLE bool is_king(int cell) const
    {
        bool result = has_piece(_white_kings, cell) || has_piece(_black_kings, cell);
        return result;
    }

    Q_INVOKABLE void restart()
    {
        reset();
    }

    Q_INVOKABLE bool has_any_piece(int cell) const
    {
        return has_white_piece(cell) || has_black_piece(cell);
    }

    Q_INVOKABLE bool has_white_piece(int cell) const
    {
        return has_piece(_white, cell);
    }

    Q_INVOKABLE bool has_black_piece(int cell) const
    {
        return has_piece(_black, cell);
    }

    Q_INVOKABLE bool can_move_from(int cell) const
    {
        return has_piece(get_state(_white_turn), cell) && (moves((1 << cell), _white_turn) != 0);
    }

    Q_INVOKABLE bool has_movable_fields() const
    {
        return current_moves() != 0;
    }

    Q_INVOKABLE bool can_move_to(int cell) const
    {
        return current_moves() & (1 << cell);
    }

    Q_INVOKABLE void move_piece_to(int cell)
    {
        int piece = activePiece();
        if (piece < 0)
            return;

        move_piece(piece, cell);
    }

    Q_INVOKABLE bool piece_can_move_to(int piece, int cell) const
    {
        return moves((1 << piece), _white_turn) & (1 << cell);
    }

    Q_INVOKABLE int activePiece() const
    {
        return _activePiece;
    }

    Q_INVOKABLE bool whiteTurn() const
    {
        return _white_turn;
    }

public slots:
    Q_INVOKABLE void setActivePiece(int i_activePiece)
    {
        if (_activePiece == i_activePiece)
            return;

        _activePiece = i_activePiece;
        emit onActivePieceChange(i_activePiece);
    }

    void setWhiteTurn(bool whiteTurn)
    {
        if (_white_turn == whiteTurn)
            return;

        _white_turn = whiteTurn;
        emit onTurnChange(whiteTurn);
    }

signals:
    void onActivePieceChange(int activePiece);
    void onTurnChange(bool whiteTurn);

private:
    void switch_turn()
    {
        _white_turn = !_white_turn;
    }

    state_t get_state(bool is_white) const
    {
        return is_white ? _white : _black;
    }

    state_t step_moves(state_t s, bool is_white) const
    {
        const state_t next = is_white ? (s<<4)|((s&0xe0e0e0e)<<3)|((s&0x707070)<<5) :
                                        (s>>4)|((s&0xe0e0e0e)>>5)|((s&0x70707070)>>3);
        state_t kings = is_white ? _white_kings : _black_kings;
        const state_t kings_moves = king_moves(s & kings);
        return ~_white & ~_black & (next | kings_moves);
    }

    state_t eat_moves(state_t s, bool is_white) const
    {
        const state_t b = get_state(!is_white);
        state_t kings = is_white ? _white_kings : _black_kings;
        if (s & kings)
        {
            const state_t kings_eat_moves = king_eat_moves(s & kings, b);
            return ~filled() & kings_eat_moves;
        }
        const state_t next =
                (((s&0x70707&(b>>4))<<9)
                |((s&0x707070&(b>>5))<<9)
                |((s&0xe0e0e0&(b>>4))<<7)
                |((s&0xe0e0e&(b>>3))<<7)
                |((s&0xe0e0e000&(b<<4))>>9)
                |((s&0xe0e0e00&(b<<5))>>9)
                |((s&0x7070700&(b<<4))>>7)
                |((s&0x70707000&(b<<3))>>7));
        return ~filled() & next;
    }

    state_t straight_moves_in_direction(state_t s, const Direction& dir) const
    {
        state_t moves = 0;
        state_t next = dir.move(s);
        while (s & ~dir._border)
        {
            moves |= next;
            s = next;
            next = dir.move(s);
        }
        return moves;
    }

    state_t king_moves_in_direction(state_t s, const Direction& dir) const
    {
        const state_t p = (_white | _black);
        state_t moves = 0;
        state_t next = dir.move(s);
        while ((next & ~p) && (s & ~dir._border))
        {
            moves |= next;
            s = next;
            next = dir.move(s);
        }
        return moves;
    }

    state_t king_moves(state_t s) const
    {
        state_t moves = 0;

        while (s)
        {
            state_t start = s & ~(s-1);
            s ^= start;

            moves |= king_moves_in_direction(start, _top_right);
            moves |= king_moves_in_direction(start, _top_left);
            moves |= king_moves_in_direction(start, _bottom_right);
            moves |= king_moves_in_direction(start, _bottom_left);
        }
        return moves;
    }

    state_t king_eat_moves_in_direction(state_t s, state_t opponent, state_t filled, const Direction& dir) const
    {
        const state_t p = filled;
        const state_t start = s;

        state_t moves = 0;
        state_t next = dir.move(s);
        while ((next & ~p) && (s & ~dir._border))
        {
            s = next;
            next = next = dir.move(s);
        }
        if ((s & ~dir._border) && (next & opponent))
        {
            s = next;
            next = next = dir.move(s);
            while ((next & ~p) && (s & ~dir._border))
            {
                moves |= next;
                s = next;
                next = next = dir.move(s);
            }
        }

        state_t continuous_eat_moves = 0;
        state_t moves_to_check = moves;
        while (moves_to_check)
        {
            state_t end = moves_to_check & ~(moves_to_check - 1);
            moves_to_check ^= end;

            state_t opp = opponent;
            state_t to_remove = (get_between(start, end) | get_between(end, start));
            opp &= ~to_remove;

            state_t filled_after_move = p;
            filled_after_move &= ~to_remove;
            filled_after_move &= ~start;
            filled_after_move |= end;

            bool can_continue = king_has_eat_moves(end, opp, filled_after_move);
            if (can_continue)
            {
                continuous_eat_moves |= end;
            }
        }

        return continuous_eat_moves ? continuous_eat_moves : moves;
    }

    bool king_has_eat_moves(state_t s, state_t opponent, state_t filled) const
    {
        state_t moves = 0;

        while (s)
        {
            state_t start = s & ~(s-1);
            s ^= start;

            moves |= king_eat_moves_in_direction(start, opponent, filled, _top_right);
            moves |= king_eat_moves_in_direction(start, opponent, filled, _top_left);
            moves |= king_eat_moves_in_direction(start, opponent, filled, _bottom_right);
            moves |= king_eat_moves_in_direction(start, opponent, filled, _bottom_left);
        }

        return moves;
    }

    state_t king_eat_moves(state_t s, state_t opponent) const
    {
        state_t moves = 0;
        const state_t p = filled();

        while (s)
        {
            state_t start = s & ~(s-1);
            s ^= start;

            moves |= king_eat_moves_in_direction(start, opponent, p, _top_right);
            moves |= king_eat_moves_in_direction(start, opponent, p, _top_left);
            moves |= king_eat_moves_in_direction(start, opponent, p, _bottom_right);
            moves |= king_eat_moves_in_direction(start, opponent, p, _bottom_left);
        }

        return moves;
    }

    state_t moves(state_t s, bool is_white) const
    {
        if (_eatingPiece >= 0)
        {
            return eat_moves(s & (1 << _eatingPiece), is_white);
        }
        return current_eat_moves() ?  eat_moves(s, is_white) :
                                      step_moves(s, is_white);
    }

    state_t current_moves() const
    {
        return moves(get_state(_white_turn), _white_turn);
    }

    state_t current_eat_moves() const
    {
        return eat_moves(get_state(_white_turn), _white_turn);
    }

    bool has_piece(state_t s, int cell) const
    {
        return s & (1 << cell);
    }

    state_t remove_piece(state_t s, int cell) const
    {
        return s & ~(1 << cell);
    }

    state_t set_piece(state_t s, int cell) const
    {
        return s | (1 << cell);
    }

    void move_piece(int piece, int cell)
    {
        const bool eat_move = current_eat_moves();

        state_t a = get_state(_white_turn);
        a = remove_piece(a, piece);
        a = set_piece(a, cell);

        state_t& kings = _white_turn ? _white_kings : _black_kings;
        if (has_piece(kings, piece))
        {
            kings = remove_piece(kings, piece);
            kings = set_piece(kings, cell);
        }

        bool become_king = (_white_turn && cell > 27) || (!_white_turn && cell < 4);
        if (become_king)
        {
            kings = set_piece(kings, cell);
        }

        if (_white_turn)
            _white = a;
        else
            _black = a;

        if (eat_move)
        {
            const state_t start = (1 << std::min(piece, cell));
            const state_t end = (1 << std::max(piece, cell));
            const state_t to_remove = get_between(start, end);

            if (!_white_turn)
            {
                _white &= ~to_remove;
                _white_kings &= ~to_remove;
            }
            else
            {
                _black &= ~to_remove;
                _black_kings &= ~to_remove;
            }
        }

        bool can_eat_more = eat_moves(1 << cell, _white_turn);
        if (eat_move && can_eat_more)
        {
            setActivePiece(cell);
            _eatingPiece = cell;
        }
        else
        {
            switch_turn();
            setActivePiece(-1);
            _eatingPiece = -1;
        }
    }

    state_t get_between(state_t start, state_t end) const
    {
        return ~(start | end) &
                (straight_moves_in_direction(start, _top_right) &
                 straight_moves_in_direction(end, _bottom_left)) |
                (straight_moves_in_direction(start, _top_left) &
                 straight_moves_in_direction(end, _bottom_right));
    }

    state_t filled() const
    {
        return _white | _black;
    }

    void reset()
    {
        _white = 0xfff;
        _black = 0xfff00000;
        _white_kings = 0;
        _black_kings = 0;
        _white_turn = true;
        _eatingPiece = -1;
        _activePiece = -1;
    }

private:
    state_t _white;
    state_t _black;
    state_t _white_kings;
    state_t _black_kings;
    bool _white_turn;
    int _eatingPiece;
    int _activePiece;

    const state_t _bottom = 0xf;
    const state_t _left = 0x1010101;
    const state_t _right = 0x80808080;
    const state_t _top = 0xf0000000;
    const Direction _top_right = Direction(4, 5, _top|_right);
    const Direction _top_left = Direction(3, 4, _top|_left);
    const Direction _bottom_right = Direction(-4, -3, _bottom|_right);
    const Direction _bottom_left = Direction(-5, -4, _bottom|_left);
};

#endif // MODEL_H
