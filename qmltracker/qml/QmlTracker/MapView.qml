import QtQuick 1.0

Item {
    id: root
    clip: true
    smooth: true
    anchors.margins: 10
    anchors.fill: parent
    property alias mapx:     content.x
    property alias mapy:     content.y
    property alias mapscale: content.scale

    signal singleTap()
    signal doubleTap()
    signal longTap()
    signal positionChanged()

    function zoomIn()   { content.zoomIn()  }
    function zoomOut()  { content.zoomOut() }
    function position() {
        return model.number(content.x,'g',0) + " " + model.number(content.y,'g',0)
    }

    ListModel {
        id: zoomlevels
        ListElement { factor: 0.3 }
        ListElement { factor: 0.5 }
        ListElement { factor: 0.7 }
        ListElement { factor: 1.0 }
        ListElement { factor: 1.4 }
        ListElement { factor: 2.0 }
        ListElement { factor: 3.0 }
    }

    Item {
        id: content
        x: 0
        y: 0
        width:  viewport.sourceSize.width
        height: viewport.sourceSize.height
        property int zoom: 3
        scale: zoomlevels.get(zoom).factor

        function setX(newx) { x = newx; positionChanged() }
        function setY(newy) { y = newy; positionChanged() }
        function zoomIn()
        {
            var mx = x; var my = y;
            zoom = zoom<6? zoom+1: zoom;
            flickable.setpos(mx,my)
        }
        function zoomOut()  {
            var mx = x; var my = y;
            zoom = zoom>0? zoom-1: zoom;
            flickable.setpos(mx,my)
        }
    }

    Image {
        id: viewport
        scale: 1
        transformOrigin: Item.TopLeft
        source: "map.jpg"
        width:  sourceSize.width     * content.scale
        height: sourceSize.height    * content.scale
        x: root.width/2  - content.x * content.scale
        y: root.height/2 - content.y * content.scale
    }

    Flickable {
        id: flickable
        width:  0
        height: 0
        anchors.left:      root.left
        anchors.right:     root.right
        anchors.bottom:    root.bottom
        anchors.top:       root.top
        contentX:          0
        contentY:          0
        contentWidth:      content.width * content.scale + root.width
        contentHeight:     content.height * content.scale + root.height
        onContentXChanged: content.setX(contentX / content.scale)
        onContentYChanged: content.setY(contentY / content.scale)
        clip: true
        MouseHandler {
            id: mouse
            onSingleTap: root.singleTap()
            onDoubleTap: root.doubleTap()
            onLongTap:   root.longTap()
        }
        function setpos (mx,my) {
            contentWidth  = content.width * content.scale + root.width
            contentHeight = content.height * content.scale + root.height
            contentX      = mx * content.scale
            contentY      = my * content.scale
        }
    }
}





