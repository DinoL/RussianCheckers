import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs


ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    title: qsTr("Russian checkers")

    MessageDialog {
        id: gameEnded
        title: "Game ended"
        text: "Someone has lost the game"
    }

    GridLayout {
        id: gridLayout
        rows: 3
        flow: GridLayout.TopToBottom
        anchors.fill: parent

        Rectangle {
            id: board
            Layout.rowSpan: gridLayout.rows
            readonly property int padSize: Math.min(root.width, root.height) / 34
            readonly property int squareSize: 4 * padSize
            property bool whiteTurn : Model.whiteTurn;
            property int curTurn: 0

            width: squareSize * 8 + 2*padSize
            height: width
            color: "#8a7b5b"
            border.color: "#DCAA33"
            border.width: 2

            Rectangle {
                x: parent.padSize - 2
                y: parent.padSize - 2
                width: parent.squareSize * 8 + 4
                height: width
                color: "#8a7b5b"
                border.color: "#DCAA33"
                border.width: 2
            }

            Repeater {
                model: 8
                Text {
                    x: board.padSize + index * board.squareSize
                    y: 0
                    text: "abcdefgh"[index]
                    width: board.squareSize
                    height: board.padSize
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                }
            }

            Repeater {
                model: 8
                Text {
                    x: board.padSize + index * board.squareSize
                    y: board.padSize + 8 * board.squareSize
                    text: "abcdefgh"[index]
                    width: board.squareSize
                    height: board.padSize
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                }
            }

            Repeater {
                model: 8
                Text {
                    x: board.padSize + 8 * board.squareSize
                    y: board.padSize + index * board.squareSize
                    text: 8 - index
                    width: board.padSize
                    height: board.squareSize
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                }
            }

            Repeater {
                model: 8
                Text {
                    x: 0
                    y: board.padSize + index * board.squareSize
                    text: 8 - index
                    width: board.padSize
                    height: board.squareSize
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                }
            }

            Rectangle {
                id: indicator

                x: board.padSize
                y: board.padSize
                z: 4

                width: board.squareSize
                height: width / 2
                visible: true

                color: "#deadbeef"

                Text {
                    width: parent.width
                    text: {
                        var cur = board.curTurn
                        return board.whiteTurn ? "white" : "black"
                    }
                    color: {
                        var cur = board.curTurn
                        return board.whiteTurn ? "white" : "black"
                    }
                    font.pointSize: board.squareSize / 4
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Repeater {
                model: 32
                Rectangle {
                    visible: true;
                    width: board.squareSize
                    height: width
                    x: board.padSize + board.squareSize*(2*(index%4)+Math.floor(index/4)%2);
                    y: board.padSize + board.squareSize*(Math.floor(index/4));
                    z: 1
                    color: "#fff0b7"
                }
            }

            Repeater {
                model: 32
                Item {
                    id: cell;
                    visible: true;
                    width: board.squareSize
                    height: width
                    x: board.padSize + board.squareSize*(2*(index%4)+Math.floor(index/4)%2);
                    y: board.padSize +  board.squareSize*(7-Math.floor(index/4));
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
                                    board.curTurn = 0
                                }
                            }
                            board.curTurn += 1
                        }
                    }
                }
            }
        }

        Button {
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: "<"
            onClicked: {
                Model.move_back()
                board.curTurn -= 1;
            }
        }
        Button {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
            text: "Restart"
            onClicked: {
                Model.restart()
                board.curTurn = 0;
            }
        }
        Button {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
            text: "Export"
            onClicked: {
                exportHistoryDialog.open()
            }
        }
        Button {
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: ">"
            onClicked: {
                Model.move_forward()
                board.curTurn += 1;
            }
        }
    }

    FileDialog {
            id: exportHistoryDialog
            defaultSuffix: "pdn"
            fileMode: FileDialog.SaveFile
            onAccepted: Model.export_history(selectedFile)
        }
}
