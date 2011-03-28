import QtQuick 1.0

Rectangle {
    id: root
    color: "black"
    visible: false

    property bool landscape: (width>height)
    property real h: (landscape)? height/360 : height/640
    property real w: (landscape)? width/640  : width/360

    property Item pageStack: null
}
