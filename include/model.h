#ifndef MODEL_H
#define MODEL_H

#include <QObject>

#include "checkerslogic.h"
#include "history.h"

using state_t = uint32_t;

class MyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int activePiece READ activePiece WRITE setActivePiece NOTIFY onActivePieceChange)
    Q_PROPERTY(bool whiteTurn READ whiteTurn WRITE setWhiteTurn NOTIFY onTurnChange)
public:
    using state_t = uint32_t;

    MyModel(QObject* parent = nullptr);

    Q_INVOKABLE bool is_king(int cell) const;
    Q_INVOKABLE void restart();
    Q_INVOKABLE bool has_any_piece(int cell) const;
    Q_INVOKABLE bool has_white_piece(int cell) const;
    Q_INVOKABLE bool has_black_piece(int cell) const;
    Q_INVOKABLE bool can_move_from(int cell) const;
    Q_INVOKABLE bool has_movable_fields() const;
    Q_INVOKABLE bool can_move_to(int cell) const;
    Q_INVOKABLE void move_piece_to(int cell);
    Q_INVOKABLE bool piece_can_move_to(int piece, int cell) const;
    Q_INVOKABLE int activePiece() const;
    Q_INVOKABLE bool whiteTurn() const;
    Q_INVOKABLE void reset_last_move();

public slots:
    Q_INVOKABLE void setActivePiece(int i_activePiece);
    void setWhiteTurn(bool whiteTurn);

signals:
    void onActivePieceChange(int activePiece);
    void onTurnChange(bool whiteTurn);

private:
    bool _whiteTurn;
    int _activePiece;

    CheckersLogic _logic;
    History _history;
};

#endif // MODEL_H
