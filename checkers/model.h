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


enum class Turn
{
    WHITE,
    BLACK
};

class MyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int activePiece READ activePiece WRITE setActivePiece NOTIFY onActivePieceChange)
public:
    using state_t = uint32_t;

    void get_mask(const std::vector<int>& v, const std::string& name)
    {
        int mask = 0;
        for(int i : v)
        {
            mask += (1 << i) + (1 << (i+8)) + (1 << (i+16));
        }
        std::cout << mask << ' ' << name << std::endl;
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

    Q_INVOKABLE bool has_piece(int cell) const
    {
        return (1 << cell) & (_white | _black);
    }

    Q_INVOKABLE bool has_white_piece(int cell) const
    {
        return (1 << cell) & _white;
    }

    Q_INVOKABLE bool has_black_piece(int cell) const
    {
        return (1 << cell) & _black;
    }

    state_t next_state(state_t state)
    {
        return ~state & (state << 4);
    }

    Q_INVOKABLE bool can_move_from(int index) const
    {
        const bool white_turn = _turn == Turn::WHITE;
        state_t a = white_turn ? _white : _black;
        state_t b = white_turn ? _black : _white;
        state_t cur = a & (1 << index);
        state_t next = white_turn ? (cur<<4)|((cur&0xe0e0e0e)<<3)|((cur&0x707070)<<5) :
                                    (cur>>4)|((cur&0xe0e0e0e)>>5)|((cur&0x70707070)>>3);

        return ~a & ~b & (next | get_eat_fields());
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

    state_t get_movable_fields() const
    {
        const state_t a = current_state();
        const state_t b = opponent_state();
        state_t next = is_white_turn() ? (a<<4)|((a&0xe0e0e0e)<<3)|((a&0x707070)<<5) :
                                         (a>>4)|((a&0xe0e0e0e)>>5)|((a&0x70707070)>>3);
        return ~a & ~b & (next | get_eat_fields());
    }

    Q_INVOKABLE bool has_movable_fields() const
    {
        return get_movable_fields() != 0;
    }

    Q_INVOKABLE bool can_move_to(int index) const
    {
        return get_movable_fields() & (1 << index);
    }

    Q_INVOKABLE bool is_white_turn() const
    {
       return _turn == Turn::WHITE;
    }

    state_t& current_state()
    {
        const bool white_turn = is_white_turn();
        return white_turn ? _white : _black;
    }

    state_t current_state() const
    {
        const bool white_turn = is_white_turn();
        return white_turn ? _white : _black;
    }

    state_t& opponent_state()
    {
        const bool white_turn = is_white_turn();
        return white_turn ? _black : _white;
    }

    state_t opponent_state() const
    {
        const bool white_turn = is_white_turn();
        return white_turn ? _black : _white;
    }

    Q_INVOKABLE void move_piece_to(int index)
    {
        int piece = activePiece();
        if (piece < 0)
            return;

        state_t& a = current_state();
        a &= ~(1 << piece);
        a |= (1 << index);
        setActivePiece(-1);
        switch_turn();

        //print();
    }

    Q_INVOKABLE bool piece_can_move_to(int piece, int index) const
    {
        const state_t a = current_state();
        const state_t b = opponent_state();
        state_t cur = a & (1 << piece);
        state_t next = is_white_turn() ? (cur<<4)|((cur&0xe0e0e0e)<<3)|((cur&0x707070)<<5) :
                                    (cur>>4)|((cur&0xe0e0e0e)>>5)|((cur&0x70707070)>>3);

        return ~a & ~b & next & (1 << index);
    }

    state_t get_eat_fields() const
    {
        return 0; // debug

        const state_t a = current_state();
        const state_t b = opponent_state();
        state_t next =
                 (((a&0x70707&(b>>4))<<(4+5))
                |(((a&0xe0e0e)<<(3+4)))
                |(((a&0x707070)<<(5+4)))
                |(((a&0xe0e0e0&(b>>4)))>>(4+3)));
        return next & ~a & ~b;
    }

    Q_INVOKABLE int activePiece() const
    {
        return _activePiece;
    }

    void switch_turn()
    {
        if (_turn == Turn::WHITE)
            _turn = Turn::BLACK;
        else
            _turn = Turn::WHITE;
    }

public slots:
    Q_INVOKABLE void setActivePiece(int i_activePiece)
    {
        if (_activePiece == i_activePiece)
            return;

        _activePiece = i_activePiece;
        emit onActivePieceChange(i_activePiece);
    }

signals:
    void onActivePieceChange(int i_activePiece);

private:
    state_t _white = 0xfff; // 134345901;
    state_t _black = 0xfff00000; //1538260992;
    Turn _turn = Turn::WHITE;
    int _activePiece;
};

#endif // MODEL_H
