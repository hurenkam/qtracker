import QtQuick 1.0

BorderImage {
    id: root

    property alias operation: buttonText.text
    property string color: ""

    signal singleTap()
    signal longTap()
    signal doubleTap()

    source: "exit.png"; clip: true
    border { left: 10; top: 10; right: 10; bottom: 10 }

    Rectangle {
        id: shade
        anchors.fill: root; radius: 10; color: "black"; opacity: 0
    }

    Text {
        id: buttonText
        anchors.centerIn: parent; anchors.verticalCenterOffset: -1
        font.pixelSize: parent.width > parent.height ? parent.height * .5 : parent.width * .5
        style: Text.Sunken; color: "white"; styleColor: "black"; smooth: true
    }

    states: State {
        name: "pressed"; when: mouseHandler.state != "idle"
        PropertyChanges { target: shade; opacity: .4 }
    }

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.singleTap()
        onLongTap: root.longTap()
        onDoubleTap: root.doubleTap()
    }

}
