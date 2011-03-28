import QtQuick 1.0

Rectangle {
    x: 3
    y: 3
    anchors.margins: 5
    radius: 5
    width: parent.width -6
    height: 32
    color: Qt.lighter(activePalette.dark)

    property alias label: labelfield.text
    property alias input: inputfield.text

    Row {
        x: 5
        y: 2
        width: parent.width -10
        height: parent.height -4

        Text {
            id: labelfield
            text: null
            color: "white"
            visible: text? true: false
        }

        Rectangle {
            color: "black"
            height: parent.height
            //anchors.margins: { 10,2,2,2 }
            anchors.left: labelfield.right
            anchors.right: parent.right
            radius: 5

            TextInput {
                x: 10
                width: parent.width -20
                id: inputfield
                text: null
                color: "white"
            }
        }
    }
}
