#include "model.h"

MyModel::MyModel(QObject* parent)
{
    Q_UNUSED(parent);
    reset();
}

bool MyModel::is_king(int cell) const
{
    bool result = has_piece(_white_kings, cell) || has_piece(_black_kings, cell);
    return result;
}

void MyModel::restart()
{
    reset();
}

bool MyModel::has_any_piece(int cell) const
{
    return has_white_piece(cell) || has_black_piece(cell);
}

bool MyModel::has_white_piece(int cell) const
{
    return has_piece(_white, cell);
}

bool MyModel::has_black_piece(int cell) const
{
    return has_piece(_black, cell);
}

bool MyModel::can_move_from(int cell) const
{
    return has_piece(get_state(_white_turn), cell) && (moves((1 << cell), _white_turn) != 0);
}

bool MyModel::has_movable_fields() const
{
    return current_moves() != 0;
}

bool MyModel::can_move_to(int cell) const
{
    return current_moves() & (1 << cell);
}

void MyModel::move_piece_to(int cell)
{
    int piece = activePiece();
    if (piece < 0)
        return;

    move_piece(piece, cell);
}

bool MyModel::piece_can_move_to(int piece, int cell) const
{
    return moves((1 << piece), _white_turn) & (1 << cell);
}

int MyModel::activePiece() const
{
    return _activePiece;
}

bool MyModel::whiteTurn() const
{
    return _white_turn;
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
    if (_white_turn == whiteTurn)
        return;

    _white_turn = whiteTurn;
    emit onTurnChange(whiteTurn);
}
