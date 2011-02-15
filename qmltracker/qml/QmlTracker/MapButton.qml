import QtQuick 1.0

Image {
    id: mapbutton
    width: 50
    height: 50
    smooth: true

    signal clicked

    MouseArea {
        id: mouseArea
        anchors.fill:  parent
        onClicked: mapbutton.clicked();
    }
}
