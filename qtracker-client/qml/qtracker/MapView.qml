import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

import QmlTrackerExtensions 1.0
import "qrc:/js/filesystem.js" as FileSystem
import "qrc:/js/helpers.js" as Helpers

Item {
    id: root
    clip: true
    smooth: true
    anchors.margins: 10
    anchors.fill: parent
    state: "scrolling"
    property alias mapx:     content.x
    property alias mapy:     content.y
    property alias maplat:   content.lat
    property alias maplon:   content.lon
    property alias mapscale: content.scale
    property alias mapname:  content.imagefile

    signal singleTap()
    signal doubleTap()
    signal longTap()
    signal positionChanged()

    function zoomIn()             { content.zoomIn()  }
    function zoomOut()            { content.zoomOut() }
    function position() {
        return Helpers.toFixed(content.lat,6) + " " + Helpers.toFixed(content.lon,6)
    }
    function loadMap(m)           { content.loadMap(m) }
    function setPosition(lat,lon) { content.setPosition(lat,lon) }

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
        x: flickable.contentX
        y: flickable.contentY
        width:  viewport.filesize.width
        height: viewport.filesize.height
        property int zoom: 3
        property string imagefile: "map.jpg"
        scale: zoomlevels.get(zoom).factor
        property real lat: refpoint.baselat + refpoint.y2lat * (y - refpoint.basey)
        property real lon: refpoint.baselon + refpoint.x2lon * (x - refpoint.basex)

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
            viewport.filename = m
            console.log("filesize: ", viewport.filesize.width, viewport.filesize.height)
            zoom = 3
            flickable.setpos(
                content.width/2,
                content.height/2
            )
            refpoint.source = FileSystem.path(m) + "/" + FileSystem.base(m) + ".xml"
        }
        XmlListModel {
            id: refpoint
            query: "/map/refpoint"
            property real baselat: 0.0
            property real baselon: 0.0
            property real basex: 0.0
            property real basey: 0.0
            property real y2lat: 0.0
            property real x2lon: 0.0
            property real lat2y: 0.0
            property real lon2x: 0.0
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
                    basex = refpoint.get(0).x
                    basey = refpoint.get(0).y
                    var dlat = refpoint.get(1).lat - refpoint.get(0).lat
                    var dlon = refpoint.get(1).lon - refpoint.get(0).lon
                    var dx = refpoint.get(1).x - refpoint.get(0).x
                    var dy = refpoint.get(1).y - refpoint.get(0).y
                    y2lat = dlat/dy
                    x2lon = dlon/dx
                    lat2y = dy/dlat
                    lon2x = dx/dlon
                }
                if (status == XmlListModel.Error) {
                    console.log("error reading refpoints")
                }
            }
        }
    }

    Item {
        id: mapmodel

        ValueSpaceSubscriber { id: lat;  path: "/server/location/latitude" }
        property double latitude: lat.value
        onLatitudeChanged: {
            console.log("onLatitudeChanged",lat.value)
            if (root.state == "followgps") {
                var y = refpoint.basey + refpoint.lat2y * (lat.value  - refpoint.baselat)
                flickable.contentY = y
                console.log("set contentY: ", y)
            }
        }

        ValueSpaceSubscriber { id: lon; path: "/server/location/longitude" }
        property double longitude: lon.value
        onLongitudeChanged: {
            console.log("onLongitudeChanged",lon.value)
            if (root.state == "followgps") {
                var x = refpoint.basex + refpoint.lon2x * (lon.value - refpoint.baselon)
                flickable.contentX = x
                console.log("set contentX: ", x)
            }
        }
    }


    MapView2 {
        id: viewport
        filename: content.imagefile
        width: root.width
        height: root.height
        mapx: content.x
        mapy: content.y
        scale: content.scale
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
        contentWidth:      content.width + root.width
        contentHeight:     content.height + root.height
        onMovementStarted: root.state = "scrolling"
        clip: true
        MouseHandler {
            id: mouse
            onSingleTap: root.state = "followgps"
            onDoubleTap: root.doubleTap()
            onLongTap:   root.longTap()
        }
        function setpos (mx,my) {
            contentWidth  = root.width  + viewport.filesize.width
            contentHeight = root.height + viewport.filesize.height
            contentX = mx
            contentY = my
        }
        function getX() {
            console.log("flickable.getX()")
            return contentX
        }
        function getY() {
            console.log("flickable.getY()")
            return contentY
        }
    }

    states: [
        State {
            name: "scrolling"
            StateChangeScript {
                script: console.log("change state to scrolling")
            }
        },
        State {
            name: "followgps"
            StateChangeScript {
                script: console.log("change state to followgps")
            }
        }
    ]
}
