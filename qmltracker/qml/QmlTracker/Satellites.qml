import QtQuick 1.0

Item {
    id: root
    width: parent.width; height: parent.height

    signal clicked()

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }

    Image {
        source: "satview.svg"
        width: parent.width
        height: parent.height
    }
}
