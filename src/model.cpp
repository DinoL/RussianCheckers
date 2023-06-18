#include "model.h"

MyModel::MyModel(QObject* parent)
{
    Q_UNUSED(parent);
    restart();
}

bool MyModel::is_king(int cell) const
{
    return _logic.has_piece(_logic.get_kings_state(true), cell) ||
           _logic.has_piece(_logic.get_kings_state(false), cell);
}

void MyModel::restart()
{
    _logic.reset();
    _whiteTurn = _logic.is_white_turn();
    _activePiece = _logic.active_piece();
}

bool MyModel::has_any_piece(int cell) const
{
    return has_white_piece(cell) || has_black_piece(cell);
}

bool MyModel::has_white_piece(int cell) const
{
    return _logic.has_piece(_logic.get_state(true), cell);
}

bool MyModel::has_black_piece(int cell) const
{
    return _logic.has_piece(_logic.get_state(false), cell);
}

bool MyModel::can_move_from(int cell) const
{
    return _logic.has_piece(_logic.get_state(_logic.is_white_turn()), cell) &&
           (_logic.moves((1 << cell), _logic.is_white_turn()) != 0);
}

bool MyModel::has_movable_fields() const
{
    return _logic.current_moves() != 0;
}

bool MyModel::can_move_to(int cell) const
{
    return _logic.current_moves() & (1 << cell);
}

void MyModel::move_piece_to(int cell)
{
    int piece = activePiece();
    if (piece < 0)
        return;

    _logic.move_piece(piece, cell);
    _whiteTurn = _logic.is_white_turn();
    _activePiece = _logic.active_piece();
}

bool MyModel::piece_can_move_to(int piece, int cell) const
{
    return _logic.moves((1 << piece), _logic.is_white_turn()) & (1 << cell);
}

int MyModel::activePiece() const
{
    return _activePiece;
}

bool MyModel::whiteTurn() const
{
    return _whiteTurn;
}

void MyModel::setActivePiece(int i_activePiece)
{
    if (_activePiece == i_activePiece)
        return;

    _activePiece = i_activePiece;
    emit onActivePieceChange(i_activePiece);
}

void MyModel::setWhiteTurn(bool whiteTurn)
{
    if (_whiteTurn == whiteTurn)
        return;

    _whiteTurn = whiteTurn;
    emit onTurnChange(whiteTurn);
}
