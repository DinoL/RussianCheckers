#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QUrl>

#include "checkerslogic.h"
#include "history.h"
#include "network_connection.h"

using state_t = uint32_t;

class MyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int activePiece READ activePiece WRITE setActivePiece NOTIFY onActivePieceChange)
public:
    MyModel(QObject* parent = nullptr);

    Q_INVOKABLE bool is_king(int cell) const;
    Q_INVOKABLE void restart();
    Q_INVOKABLE bool has_any_piece(int cell) const;
    Q_INVOKABLE bool has_white_piece(int cell) const;
    Q_INVOKABLE bool has_black_piece(int cell) const;
    Q_INVOKABLE bool was_eaten(int cell) const;
    Q_INVOKABLE bool can_move_from(int cell) const;
    Q_INVOKABLE bool has_movable_fields() const;
    Q_INVOKABLE bool can_move_to(int cell) const;
    Q_INVOKABLE void move_piece_to(int cell);
    Q_INVOKABLE bool piece_can_move_to(int piece, int cell) const;
    Q_INVOKABLE int activePiece() const;
    Q_INVOKABLE bool whiteTurn() const;
    Q_INVOKABLE void export_history(const QUrl& file_url) const;
    Q_INVOKABLE void import_history(const QUrl& file_url);
    Q_INVOKABLE void move_back();
    Q_INVOKABLE void move_forward();
    Q_INVOKABLE int piece_count(bool is_white) const;
    Q_INVOKABLE void connect();

public slots:
    Q_INVOKABLE void setActivePiece(int i_activePiece);

signals:
    void onActivePieceChange(int activePiece);

protected:
    void set_from_history(int turn);

private:
    bool _whiteTurn;
    int _activePiece;
    int _curTurn;

    CheckersLogic _logic;
    History _history;
    NetworkConnection _connection;
};

#endif // MODEL_H
