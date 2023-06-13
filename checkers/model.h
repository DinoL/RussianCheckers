#ifndef MODEL_H
#define MODEL_H

#include <QObject>


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
    }

    Q_INVOKABLE bool has_any_piece(int cell) const
    {
        return has_white_piece(cell) || has_black_piece(cell);
    }

    Q_INVOKABLE bool has_white_piece(int cell) const
    {
        return has_piece(cell, true);
    }

    Q_INVOKABLE bool has_black_piece(int cell) const
    {
        return has_piece(cell, false);
    }

    Q_INVOKABLE bool can_move_from(int cell) const
    {
        return has_piece(cell, _white_turn) && (moves((1 << cell), _white_turn) != 0);
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

        const bool eat_move = current_eat_moves();

        state_t a = get_state(_white_turn);
        state_t b = get_state(!_white_turn);
        a = remove_piece(a, piece);
        a = set_piece(a, cell);

        if (_white_turn)
            _white = a;
        else
            _black = a;

        if (eat_move)
        {
            int center = get_center(piece, cell);
            b = remove_piece(b, center);
            if (!_white_turn)
                _white = b;
            else
                _black = b;
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
        return ~_white & ~_black & next;
    }

    state_t eat_moves(state_t s, bool is_white) const
    {
        const state_t b = get_state(!is_white);
        const state_t next =
                (((s&0x70707&(b>>4))<<9)
                |((s&0x707070&(b>>5))<<9)
                |((s&0xe0e0e0&(b>>4))<<7)
                |((s&0xe0e0e&(b>>3))<<7)
                |((s&0xe0e0e000&(b<<4))>>9)
                |((s&0xe0e0e00&(b<<5))>>9)
                |((s&0x7070700&(b<<4))>>7)
                |((s&0x70707000&(b<<3))>>7));
        return ~_white & ~_black & next;
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

    bool has_piece(int cell, bool is_white) const
    {
        return (1 << cell) & get_state(is_white);
    }

    state_t remove_piece(state_t s, int cell) const
    {
        return s & ~(1 << cell);
    }

    state_t set_piece(state_t s, int cell) const
    {
        return s | (1 << cell);
    }

    int get_center(int start, int end) const
    {
        return (start + end + (start / 4) % 2) / 2;
    }

private:
    state_t _white = 0xfff;
    state_t _black = 0xfff00000;
    bool _white_turn = true;
    int _eatingPiece = -1;
    int _activePiece = -1;
};

#endif // MODEL_H
