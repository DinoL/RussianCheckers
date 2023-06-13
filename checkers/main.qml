import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 600
    height: 600
    title: qsTr("Russian checkers")

    Rectangle {
        id: board
        readonly property int squareSize: Math.min(root.width, root.height) / 8;
        property int hover: -1;
        property bool whiteTurn : Model.is_white_turn();
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
                text: board.whiteTurn ? "white" : "black"
                color: board.whiteTurn ? "white" : "black"
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
                property bool has_piece: Model.has_piece(index);
                readonly property real gap: 0.075;

                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: {
                        var data = board.curTurn
                        //var suffix = ""
                        //if (data > 1000)
                        //    suffix += ""
                        //console.log("color", index, board.hover)
                        if (board.needUpate)
                            suffix += ""

                        if (index === Model.activePiece)
                            return "#a43b00";

                        if (index === board.hover)
                            return "#ff743f";

                        var can_move_to = Model.can_move_to(index);
                        var can_move_from = Model.can_move_from(index);
                        if (Model.activePiece < 0 && can_move_from)
                            return "#e0856a";

                        if (Model.activePiece >= 0 && Model.piece_can_move_to(Model.activePiece, index))
                            return "#e0856a";

                        return "#ee8d22" // + suffix;
                    }
                }
                Rectangle {
                    id: man
                    x: board.squareSize * cell.gap;
                    y: board.squareSize * cell.gap;
                    width: board.squareSize*(1.0 - 2.0 * cell.gap)
                    height: width
                    color: {
                        Model.has_white_piece(index) ? "#ffe" : "#444"
                    }
                    border.color: "black"
                    border.width: 1
                    radius: width*0.5
                    visible: {
                        var cur = board.curTurn;
                        if (cur < 10000)
                            return Model.has_piece(index)
                        return true
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
                    hoverEnabled: true

                    onPressed: {
                        //console.log("man visibility before", index, cell.has_piece)
                        if (Model.can_move_from(index)) {
                            var to_set = Model.activePiece === index ? -1 : index
                            Model.setActivePiece(to_set);
                        }
                        if (Model.activePiece >= 0 && Model.piece_can_move_to(Model.activePiece, index)) {
                            //console.log("Move", Model.activePiece, "to", index)
                            Model.move_piece_to(index);
                            board.whiteTurn = Model.is_white_turn();
                            //root.update()
                            //console.log("man visibility after", index, cell.has_piece)
                            cell.has_piece = Model.has_piece(index)
                            //man.visible = Model.has_piece(index)
                            man.color = Model.has_white_piece(index) ? "#ffe" : "#444"

                            if (!Model.has_movable_fields())
                            {
                                var player_name = board.whiteTurn ? "white" : "black"
                                console.log(player_name + " has lost the game")
                            }
                        }
                        board.curTurn += 1
                    }
                    onEntered: {
                        if (cell.has_piece) {
                            board.hover = index
                        }
                    }
                    onExited: {
                        if (cell.has_piece) {
                            board.hover = -1
                        }
                    }
                }
            }
        }
    }
}
