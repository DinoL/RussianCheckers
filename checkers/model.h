#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtQml/qqml.h>
#include <QQmlEngine>
#include <QtQuick/QQuickItem>
#include <iostream>
#include <bitset>
#include <intrin.h>
#include <stdio.h>
#include <vector>
#include <string>


class MyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int activePiece READ activePiece WRITE setActivePiece NOTIFY onActivePieceChange)
    Q_PROPERTY(bool whiteTurn READ whiteTurn WRITE setWhiteTurn NOTIFY onTurnChange)
public:
    using state_t = uint32_t;

    void get_mask(const std::vector<int>& v, const std::string& name)
    {
        state_t mask = 0;
        for(int i : v)
        {
            mask += (1 << i) + (1 << (i+8)) + (1 << (i+16));
        }
        std::cout << std::hex << mask << ' ' << name << std::endl;
    }

    MyModel(QObject* parent = nullptr)
    {
        Q_UNUSED(parent);

        //std::cout << "Model created" << std::endl;
        //std::bitset<32> xw(_white);
        //std::bitset<32> xb(_black);
        //std::cout << "White: " << xw << " (" << _white << ')' << std::endl;
        //std::cout << "Black: " << xb << " (" << _black << ')' << std::endl;
        //
        //get_mask({0,1,2}, "4+5");
        //get_mask({4,5,6}, "5+4");
        //get_mask({1,2,3}, "3+4");
        //get_mask({5,6,7}, "4+3");

        //print();
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

    void print() const {
        std::string black_dot = "8";
        std::string white_dot = "o";
        std::string empty = "_";

        for (int row = 7; row >= 0; --row)
        {
            for (int col = 0; col < 8; ++col)
            {
                if ((col + row) % 2 == 0)
                {
                    int index = 4*row + (col / 2);
                    if (_white & (1 << index))
                    {
                        std::cout << white_dot;
                        continue;
                    }
                    if (_black & (1 << index))
                    {
                        std::cout << black_dot;
                        continue;
                    }

                }
                std::cout << empty;
            }
            std::cout << std::endl;
        }
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

        state_t a = get_state(_white_turn);
        a &= ~(1 << piece);
        a |= (1 << cell);
        setActivePiece(-1);

        if (_white_turn)
            _white = a;
        else
            _black = a;

        switch_turn();

//        std::bitset<32> xw(_white);
//        std::bitset<32> xb(_black);
//        std::cout << "White: " << xw << " (" << _white << ')' << std::endl;
//        std::cout << "Black: " << xb << " (" << _black << ')' << std::endl;

        //print();
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
        state_t e = eat_moves(s, is_white);
        return e ? e : step_moves(s, is_white);
    }

    state_t current_moves() const
    {
        return moves(get_state(_white_turn), _white_turn);
    }

    bool has_piece(int cell, bool is_white) const
    {
        return (1 << cell) & get_state(is_white);
    }

private:
    state_t _white = 0xfff; //0b100000; // 134345901;
    state_t _black = 0xfff00000; // 0b1000000000; //1538260992;
    bool _white_turn = true;
    int _activePiece = -1;
};

#endif // MODEL_H
