import QtQuick 1.0

Item {}

/* Obsolete

Item {
    id: root
    property alias text: priv.text
    property bool selected: false

    Image {
        x: 4
        y: 4
        width: root.height-8
        height: root.height-8
        source: root.selected? "qrc:/images/visible.svg" : "qrc:/images/invisible.svg"
    }

    Text {
        id: priv
        text: null
        x: root.height
        y: 2
        height: root.height
        width: root.width - 36
        font.pixelSize: root.height/1.6
        color: "white"
    }
}
*/
