import QtQuick 1.0

Rectangle {
    id: root
    property bool active: false
    visible: active
    opacity: active? 1 : 0
    anchors.fill: parent
    color: "black"

    property real w: (height>width)? width/360 : width/640
    property real h: (height>width)? height/640 : height/360
    property bool landscape: (width>height)

    property Item pageStack: null

    function activate(newparent) {
        console.log("Page:activate")
        root.parent = newparent;
        root.active = true;
    }

    function deActivate() {
        console.log("Page:deActivate")
        root.active = false
        root.parent = null
    }
}
