import QtQuick 1.0

Item {
    id: root
    property alias text: priv.text
    property bool selected: false

    Image {
        x: 4
        y: 4
        width: 20
        height: 20
        source: root.selected? "qrc:/images/visible.svg" : "qrc:/images/invisible.svg"
    }

    Text {
        id: priv
        text: null
        x: 32
        y: 2
        height: 20
        width: parent.width - 36
        color: "white"
    }
}
