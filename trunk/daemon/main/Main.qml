import QtQuick 1.0

Rectangle {
    width: 360
    height: 360

    Text {
        text: qsTr("Keep Track!")
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Connections {
        target:       daemon
        onQuit:       { console.log("daemon.onQuit()"); Qt.quit() }
    }
}
