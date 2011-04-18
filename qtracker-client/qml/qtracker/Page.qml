import QtQuick 1.0

Rectangle {
    id: root
    color: "black"
    visible: false
    property alias imageSource: image.source

    Image {
        id: image
        anchors.fill: parent
        visible: (source != "")
        source: ""
        fillMode: Image.Tile
    }

    property bool landscape: (width>height)
    property real h: (landscape)? height/360 : height/640
    property real w: (landscape)? width/640  : width/360

    property Item pageStack: null
    //onWidthChanged:  console.log ("Page.onWidthChanged",width,height)
    //onHeightChanged: console.log ("Page.onHeightChanged",width,height)
}
