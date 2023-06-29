#include "model.h"
#include "bit_algo.h"
#include "pdnparser.h"

#include <fstream>

#include <QString>

MyModel::MyModel(QObject* parent)
{
    Q_UNUSED(parent);
    restart();
}

bool MyModel::is_king(int cell) const
{
    return alg::has_piece(_logic.get_kings_state(true), cell) ||
           alg::has_piece(_logic.get_kings_state(false), cell);
}

void MyModel::restart()
{
    _logic.reset();
    _whiteTurn = _logic.is_white_turn();
    _activePiece = -1;
    _curTurn = 0;

    _history.clear();
    _history.push(_logic.state());
}

bool MyModel::has_any_piece(int cell) const
{
    return alg::has_piece(_logic.filled(), cell);
}

bool MyModel::has_white_piece(int cell) const
{
    return alg::has_piece(_logic.get_state(true), cell);
}

bool MyModel::has_black_piece(int cell) const
{
    return alg::has_piece(_logic.get_state(false), cell);
}

bool MyModel::can_move_from(int cell) const
{
    return alg::has_piece(_logic.get_state(_logic.is_white_turn()), cell) &&
           (_logic.moves(alg::to_state(cell), _logic.is_white_turn()) != 0);
}

bool MyModel::has_movable_fields() const
{
    return _logic.current_moves() != 0;
}

bool MyModel::can_move_to(int cell) const
{
    return _logic.current_moves() & alg::to_state(cell);
}

void MyModel::move_piece_to(int cell)
{
    int piece = activePiece();
    if (piece < 0)
        return;

    _logic.move_piece(piece, cell);
    _whiteTurn = _logic.is_white_turn();
    _activePiece = _logic.eating_piece();
    if (_curTurn != (_history.size() - 1))
    {
        _history.resize(_curTurn + 1);
    }
    _history.push(_logic.state());
    ++_curTurn;
}

bool MyModel::piece_can_move_to(int piece, int cell) const
{
    return _logic.moves(alg::to_state(piece), _logic.is_white_turn()) & alg::to_state(cell);
}

int MyModel::activePiece() const
{
    return _activePiece;
}

bool MyModel::whiteTurn() const
{
    return _whiteTurn;
}

void MyModel::export_history(const QUrl& file_url) const
{
    std::ofstream file(
        file_url.toDisplayString(QUrl::PreferLocalFile).toStdString());
    const PdnParser parser(CellFormat::ALPHANUMERIC);
    parser.write(PDN::from_history(_history), file);
}

void MyModel::import_history(const QUrl& file_url)
{
    std::ifstream file(
        file_url.toDisplayString(QUrl::PreferLocalFile).toStdString());
    const PdnParser parser(CellFormat::ALPHANUMERIC);
    const PDN pdn = parser.read(file);
    _history = pdn.to_history();
    _logic.set_state(_history[0]);
    _activePiece = -1;
    _curTurn = 0;
}

void MyModel::move_back()
{
    if (_curTurn <= 0)
    {
        return;
    }

    --_curTurn;
    _logic.set_state(_history[_curTurn]);
    _activePiece = -1;
}

void MyModel::move_forward()
{
    if (_curTurn >= _history.size() - 1)
    {
        return;
    }

    ++_curTurn;
    _logic.set_state(_history[_curTurn]);
    _activePiece = -1;
}

int MyModel::piece_count(bool is_white) const
{
    return alg::count(_logic.get_state(is_white));
}

void MyModel::setActivePiece(int i_activePiece)
{
    if (_activePiece == i_activePiece)
        return;

    _activePiece = i_activePiece;
    emit onActivePieceChange(i_activePiece);
}

