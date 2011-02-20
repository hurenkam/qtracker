import QtQuick 1.0
import "qrc:/js/filesystem.js" as FileSystem

Item {
    id: root
    clip: true
    smooth: true
    anchors.margins: 10
    anchors.fill: parent
    property alias mapx:     content.x
    property alias mapy:     content.y
    property alias maplat:   content.lat
    property alias maplon:   content.lon
    property alias mapscale: content.scale
    property alias mapname:  content.imagefile
    property alias status:   viewport.status

    signal singleTap()
    signal doubleTap()
    signal longTap()
    signal positionChanged()

    function zoomIn()   { content.zoomIn()  }
    function zoomOut()  { content.zoomOut() }
    function position() {
        //return model.number(content.x,'g',0) + " " + model.number(content.y,'g',0)
        return model.number(content.lat,'g',6) + " " + model.number(content.lon,'g',6)
    }
    function loadMap(m) { content.loadMap(m) }

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
        property string imagefile: "map.jpg"
        scale: zoomlevels.get(zoom).factor
        property real lat: refpoint.baselat + refpoint.y2lat * y
        property real lon: refpoint.baselon + refpoint.x2lon * x

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
        function loadMap(m) {
            imagefile = m
            zoom = 3
            flickable.setpos(
                viewport.sourceSize.width/2,
                viewport.sourceSize.height/2
            )
            refpoint.source = FileSystem.path(m) + "/" + FileSystem.base(m) + ".xml"
        }
        XmlListModel {
            id: refpoint
            query: "/map/refpoint"
            property real baselat: 0.0
            property real baselon: 0.0
            property real y2lat: 0.0
            property real x2lon: 0.0
            XmlRole { name: "lat"; query: "@lat/number()" }
            XmlRole { name: "lon"; query: "@lon/number()" }
            XmlRole { name: "x";   query: "@x/number()"   }
            XmlRole { name: "y";   query: "@y/number()"   }

            onStatusChanged: {
                if (status == XmlListModel.Ready) {
                    console.log("refpoint:   ",refpoint.get(0).x,refpoint.get(0).y,refpoint.get(0).lat,refpoint.get(0).lon)
                    console.log("refpoint:   ",refpoint.get(1).x,refpoint.get(1).y,refpoint.get(1).lat,refpoint.get(1).lon)
                    baselat = refpoint.get(0).lat
                    baselon = refpoint.get(0).lon
                    var dlat = refpoint.get(1).lat - refpoint.get(0).lat
                    var dlon = refpoint.get(1).lon - refpoint.get(0).lon
                    var dx = refpoint.get(1).x - refpoint.get(0).x
                    var dy = refpoint.get(1).y - refpoint.get(0).y
                    y2lat = dlat/dy
                    x2lon = dlon/dx
                }
                if (status == XmlListModel.Error) {
                    console.log("error reading refpoints")
                }
            }
        }
    }

    Image {
        id: viewport
        scale: 1
        transformOrigin: Item.TopLeft
        source: content.imagefile
        width:  sourceSize.width     * content.scale
        height: sourceSize.height    * content.scale
        x: root.width/2  - content.x * content.scale
        y: root.height/2 - content.y * content.scale
/*        onStatusChanged: {
            width=  sourceSize.width     * content.scale
            height= sourceSize.height    * content.scale
            x= root.width/2  - content.x * content.scale
            y= root.height/2 - content.y * content.scale
        }*/
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
