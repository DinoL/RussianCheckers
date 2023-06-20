import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs


ApplicationWindow {
    id: root
    visible: true
    width: 600
    height: 600
    title: qsTr("Russian checkers")

    MessageDialog {
        id: gameEnded
        title: "Game ended"
        text: "Someone has lost the game"
    }

    Rectangle {
        id: board
        readonly property int squareSize: Math.min(root.width, root.height) / 8;
        property bool whiteTurn : Model.whiteTurn;
        property int curTurn: 0

        width: squareSize * 8
        height: width
        color: "#fff0b7"

        Rectangle {
            id: indicator

            width: board.squareSize
            height: width / 2

            color: {
                "#deadbeef"
            }

            Text {
                text: {
                    var cur = board.curTurn
                    return board.whiteTurn ? "white" : "black"
                }
                color: {
                    var cur = board.curTurn
                    return board.whiteTurn ? "white" : "black"
                }
                font.pointSize: board.squareSize / 4
            }
        }

        Repeater {
            model: 32
            Item {
                id: cell;
                visible: true;
                width: board.squareSize
                height: width
                x: board.squareSize*(2*(index%4)+Math.floor(index/4)%2);
                y: board.squareSize*(7-Math.floor(index/4));
                z: 1
                property bool has_piece: Model.has_any_piece(index);
                readonly property real gap: 0.075;

                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: {
                        var data = board.curTurn
                        if (index === Model.activePiece)
                            return "#a43b00";

                        var can_move_from = Model.can_move_from(index);
                        if (Model.activePiece < 0 && can_move_from)
                            return "#e0856a";

                        if (Model.activePiece >= 0 && Model.piece_can_move_to(Model.activePiece, index))
                            return "#3fbf00"

                        return "#ee8d22"
                    }
                }
                Rectangle {
                    id: man
                    x: board.squareSize * cell.gap;
                    y: board.squareSize * cell.gap;
                    width: board.squareSize*(1.0 - 2.0 * cell.gap)
                    height: width
                    color: {
                        var cur = board.curTurn
                        Model.has_white_piece(index) ? "#ffe" : "#444"
                    }
                    border.color: "black"
                    border.width: 1
                    radius: width*0.5
                    visible: {
                        var cur = board.curTurn;
                        return Model.has_any_piece(index)
                    }

                    Rectangle {
                        width: parent.width * 0.7
                        height: width
                        color: {
                            var cur = board.curTurn
                            Model.has_white_piece(index) ? "blue" : "red"
                        }
                        border.color: "black"

                        x: (parent.width - width) * 0.5
                        y: x
                        z: 2
                        radius: width*0.5
                        visible: {
                            var cur = board.curTurn;
                            return Model.is_king(index) ? true : false;
                        }
                    }

                    Rectangle {
                        width: parent.width * 0.7
                        height: width
                        color: parent.color
                        x: (parent.width - width) * 0.5
                        y: x
                        border.color: parent.border.color
                        border.width: 1
                        radius: width*0.5

                        Rectangle {
                            width: parent.width * 0.5
                            height: width
                            color: parent.color
                            x: (parent.width - width) * 0.5
                            y: x
                            border.color: parent.border.color
                            border.width: 1
                            radius: width*0.5
                        }
                    }
                }
                MouseArea {
                    id: cellTouch;
                    width: parent.width
                    height: parent.height

                    onPressed: {
                        if (Model.can_move_from(index)) {
                            var to_set = Model.activePiece === index ? -1 : index
                            Model.setActivePiece(to_set);
                        }
                        if (Model.activePiece >= 0 && Model.piece_can_move_to(Model.activePiece, index)) {
                            Model.move_piece_to(index);
                            board.whiteTurn = Model.whiteTurn;
                            cell.has_piece = Model.has_any_piece(index)

                            if (!Model.has_movable_fields())
                            {
                                var player_name = board.whiteTurn ? "White" : "Black"
                                gameEnded.text = (player_name + " has lost the game")
                                gameEnded.open()
                                Model.restart()
                            }
                        }
                        board.curTurn += 1
                    }
                }
            }
        }
    }
}
