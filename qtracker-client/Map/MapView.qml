import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import QmlTrackerExtensions 1.0
import "../Components"

Item {
    id: root
    clip: true
    smooth: true
//    anchors.margins: 10
//    anchors.fill: parent
    //state: "scrolling"
    property alias mapx:     content.x
    property alias mapy:     content.y
    property alias maplat:   content.lat
    property alias maplon:   content.lon
    property alias mapscale: content.scale
    property alias mapname:  viewport.name
    property alias mapid:    content.mapid

    function lat2viewy(v) { return (content.lat2mapy(v) - content.y)*content.scale + viewport.height/2 }
    function lon2viewx(v) { return (content.lon2mapx(v) - content.x)*content.scale + viewport.width/2  }
    function isPositionInView(lat,lon) {
        var vy = lat2viewy(lat)
        var vx = lon2viewx(lon)
        var result = ( (vy>=0) && (vy<=viewport.height) && (vx>=0) && (vx<=viewport.width) )
        console.log("MapView.isPositionInView",lat,lon,vy,vx,result)
        return result
    }

    function drawWaypoints(points) {
    }
    function drawTrackpoints(points) {
    }
    function drawRoute(points) {
    }

    signal singleTap()
    signal doubleTap()
    signal longTap()
    signal positionChanged()
    signal zoomChanged()
    signal mapLoaded(int mapid, string name)

    function zoomIn()             { content.zoomIn()  }
    function zoomOut()            { content.zoomOut() }
    function position() {
        return Helpers.toFixed(content.lat,6) + " " + Helpers.toFixed(content.lon,6)
    }
    //function loadMap(m)           { content.loadMap(m) }
    function loadMap(id)          { content.loadMap(id) }
    function setPosition(lat,lon) { content.setPosition(lat,lon) }
/*
    function base(filename) {
        var txt = String(filename);
        var p1 = txt.lastIndexOf('/');
        var p2 = txt.lastIndexOf('.');
        return txt.slice(p1+1,p2);
    }

    function path(filename) {
        var txt = String(filename);
        var p = txt.lastIndexOf('/');
        return txt.slice(0,p);
    }

    function ext(filename) {
        var txt = String(filename);
        var p = txt.lastIndexOf('.');
        return txt.slice(p+1);
    }
*/
    function clearCache() {
        viewport.clearcache()
    }

    Settings {
        id: settings
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
        x: flickable.contentX
        y: flickable.contentY
        width:  viewport.filesize.width
        height: viewport.filesize.height
        property int zoom: 3
        property int mapid: -1
        property string imagefile: ""
        scale: zoomlevels.get(zoom).factor
        //property real lat: refpoint.baselat + refpoint.y2lat * (y - refpoint.basey)
        //property real lon: refpoint.baselon + refpoint.x2lon * (x - refpoint.basex)
        property double lat: viewport.isCalibrated? viewport.y2lat(y) : mapmodel.latitude
        property double lon: viewport.isCalibrated? viewport.x2lon(x) : mapmodel.longitude
        //property double lat: mapy2lat(y)
        //property double lon: mapx2lon(x)

        //function mapy2lat(v)  { return refpoint.baselat + refpoint.y2lat * (v - refpoint.basey)   }
        //function mapx2lon(v)  { return refpoint.baselon + refpoint.x2lon * (v - refpoint.basex)   }
        //function lat2mapy(v)  { return refpoint.basey   + refpoint.lat2y * (v - refpoint.baselat) }
        //function lon2mapx(v)  { return refpoint.basex   + refpoint.lon2x * (v - refpoint.baselon) }

        function zoomIn()
        {
            var mx = x; var my = y;
            zoom = zoom<6? zoom+1: zoom;
            //settings.setProperty("map_zoom", zoom)
            flickable.setpos(mx,my)
            root.zoomChanged()
        }
        function zoomOut()  {
            var mx = x; var my = y;
            zoom = zoom>0? zoom-1: zoom;
            //settings.setProperty("map_zoom", zoom)
            flickable.setpos(mx,my)
            root.zoomChanged()
        }
        function _loadMap(i) {
            mapid = i
            settings.setProperty("map_id", i);
        }
        function loadMap(i) {
            _loadMap(i)
            console.log("filesize: ", viewport.filesize.width, viewport.filesize.height)
            zoom = 3
            flickable.setpos(
                content.width/2,
                content.height/2
            )
        }

        Component.onCompleted: {
            var i = settings.getProperty("map_id",-1)
            if (i != -1) {
                _loadMap(i);
            }
            var x = settings.getProperty("map_x",0)
            var y = settings.getProperty("map_y",0)
            zoom = settings.getProperty("map_zoom",3)
            flickable.setpos(x,y)
        }
    }

    Item {
        id: mapmodel

        ValueSpaceSubscriber { id: lat;  path: "/server/location/latitude" }
        property double latitude: lat.value
        onLatitudeChanged: {
            console.log("onLatitudeChanged",lat.value)
            if (root.state == "followgps") {
                //var y = refpoint.basey + refpoint.lat2y * (lat.value  - refpoint.baselat)
                var y = viewport.lat2y(lat.value)
                flickable.contentY = y
                console.log("set contentY: ", y)
            }
        }

        ValueSpaceSubscriber { id: lon; path: "/server/location/longitude" }
        property double longitude: lon.value
        onLongitudeChanged: {
            console.log("onLongitudeChanged",lon.value)
            if (root.state == "followgps") {
                //var x = refpoint.basex + refpoint.lon2x * (lon.value - refpoint.baselon)
                var x = viewport.lon2x(lon.value)
                flickable.contentX = x
                console.log("set contentX: ", x)
            }
        }
    }


    MapView2 {
        id: viewport
        //filename: content.imagefile
        mapid: content.mapid
        width: root.width
        height: root.height
        mapx: content.x
        mapy: content.y
        scale: content.scale
        onFilenameChanged: {
            console.log("MapView.viewport.onFilenameChanged")
            root.mapLoaded(content.mapid, viewport.name)
        }
    }

    Timer {
        id: timer
        interval: 5000
        running: false
        repeat: false
        onTriggered: {
            settings.setProperty("map_x",flickable.contentX)
            settings.setProperty("map_y",flickable.contentY)
            settings.setProperty("map_zoom", content.zoom)
        }
    }

    Flickable {
        id: flickable
        width:  0
        height: 0
        anchors.left:      root.left
        anchors.right:     root.right
        anchors.bottom:    root.bottom
        anchors.top:       root.top
        contentX: 0
        contentY: 0
        contentWidth:      content.width + root.width
        contentHeight:     content.height + root.height
        onMovementStarted: { timer.stop(); root.state = "scrolling" }
        onMovementEnded:   { timer.start(); }

        clip: true
        property bool initialized: false

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
            settings.setProperty("map_x",flickable.contentX)
            settings.setProperty("map_y",flickable.contentY)
        }
        function getX() {
            console.log("flickable.getX()")
            return contentX
        }
        function getY() {
            console.log("flickable.getY()")
            return contentY
        }
        Component.onCompleted: {
            flickable.initialized = true
        }
    }

    states: [
        State {
            name: "scrolling"
            StateChangeScript {
                script: { console.log("change state to scrolling"); settings.setProperty("map_state","scrolling") }
            }
        },
        State {
            name: "followgps"
            StateChangeScript {
                script: { console.log("change state to followgps"); settings.setProperty("map_state","followgps") }
            }
        }
    ]

    Component.onCompleted: {
        root.state = settings.getProperty("map_state","scrolling")
    }
}
