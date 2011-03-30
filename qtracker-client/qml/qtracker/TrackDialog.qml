import QtQuick 1.0

Page {
    id: root

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }

    TrackEdit {
        id: edit
    }

    function newtrack() {
        pageStack.push(edit)
    }

    Rectangle {
        id: heading

        x: 2
        y: 2
        width: parent.width -4
        height:  50
        radius:  15
        color: activePalette.light
        state: "normal"

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: state=="pressed"? Qt.darker(activePalette.light) : Qt.lighter(activePalette.light)
            }
            GradientStop {
                position:  1.0
                color: state=="pressed"? Qt.darker(activePalette.dark) : Qt.lighter(activePalette.dark)
            }
        }

        ToolButton {
            x: 5
            y: 5
            width: 40
            height: 40

            source: "qrc:/images/back.svg";
            onClicked: root.cancel();
        }

        Text {
            x: 70; y: 5
            text: "Track Menu"
            font.pixelSize: parent.height/2
            color: "white"
            visible: text? true: false
        }

        MouseArea {
            anchors.fill:  parent
            onClicked: root.cancel()
            onPressed: state="pressed"
            onCanceled: state="normal"
            onReleased: state="normal"
        }
    }


    Rectangle {
        id: newtrack

        x: 2
        y: 56
        width: parent.width -4
        height:  50
        radius:  15
        color: activePalette.light
        state: "normal"

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: state=="pressed"? Qt.darker(activePalette.light) : Qt.lighter(activePalette.light)
            }
            GradientStop {
                position:  1.0
                color: state=="pressed"? Qt.darker(activePalette.dark) : Qt.lighter(activePalette.dark)
            }
        }

        Text {
            x: 70; y: 5
            text: "New Track"
            font.pixelSize: parent.height/2
            color: "white"
            visible: text? true: false
        }

        ToolButton {
            x: parent.width -45
            y: 5
            width: 40
            height: 40

            source: "qrc:/images/forward.svg";
            onClicked: root.newtrack();
        }

        MouseArea {
            anchors.fill:  parent
            onClicked: root.newtrack()
            onPressed: state="pressed"
            onCanceled: state="normal"
            onReleased: state="normal"
        }
    }

    Rectangle {
        id: listbox

        x: 2
        y: 110
        width: parent.width -4
        height: parent.height -112
        radius:  15
        color: activePalette.light

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: activePalette.light
            }
            GradientStop {
                position:  1.0
                color: activePalette.dark
            }
        }

        TrackList {
            id: tracklist
            anchors.fill: parent
        }
    }
}
