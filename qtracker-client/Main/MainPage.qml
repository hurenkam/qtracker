import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import QmlTrackerExtensions 1.0
import "../Components"
import "../Gauges"
import "../Category"
import "../Map"
import "../Waypoint"
import "../Route"
import "../Track"
import "../Trip"

Page {
    id: root
    property bool animate: false

    function exitServer() {
        console.log("exitServer()")
        var cmd = { "class": "server", "method": "stop", "args": [] }
        client.sendCommand(cmd);
    }
    function exitClient() {
        console.log("exitClient()")
        Qt.quit();
    }
    function exitClientAndServer() {
        console.log("exitClientAndServer()")
        root.exitServer();
        root.exitClient();
    }

    Settings              { id: settings }

    Component { id: catselectsrc;  CategoryEditPage  { id: catPage;  } }
    Component { id: tripselectsrc; TripEditPage      { id: tripPage; } }
    Component { id: tstselectsrc;  ImExportPage      { id: tstPage;  } }
    Component { id: mapselectsrc;
        MapEditPage {
            id: mapPage;
            mapid: map.mapid
            onMapSelected: map.loadMap(mapid);
        }
    }

    Loader {
        id: pageloader
    }

    function showPage(src) {
        pageloader.sourceComponent = src
        //pageloader.item.setupMapArguments(map.mapid,map.mapname,map.maplat,map.maplon,0.0,map.mapx,map.mapy)
        pageStack.push(pageloader.item)
    }

    MapView {
        id: map
        anchors.top:    toolbar.bottom;
        anchors.bottom: dashboard.top;
        width: parent.width;
        clip: true;
        onSingleTap: root.singleTap()
        onDoubleTap: root.doubleTap()
        onLongTap:   root.longTap()

        Image {
            id: locator
            source: map.state == "scrolling"? "locator_red.svg" : "locator_green.svg"
            width: sourceSize.width/2
            height: sourceSize.height/2
            x: (parent.width-width)/2
            y: (parent.height-height)/2
        }

        onMapLoaded: {
            //mapPage.currentmap = name
            //mapPage.currentid = mapid
            console.log("MapView.onMapLoaded()",mapid)
            //mapPage.setupMapArguments(map.mapid,map.mapname,map.maplat,map.maplon,map.mapx,map.mapy)
            //if (pageloader.item)
            //    pageloader.item.setupMapArguments(map.mapid,map.mapname,map.maplat,map.maplon,0.0,map.mapx,map.mapy)
        }
    }

    ToolBar {
        id: toolbar
        property int animationDuration: 300
        Behavior on height {
            enabled: root.animate
            NumberAnimation { easing.type: Easing.InOutQuart; duration: toolbar.animationDuration }
        }

        function toggleHide() {
            hide = hide? false: true;
            settings.setProperty("toolbar_hide", hide)
        }

        tools: ToolBarLayout {
            id: maintools
            ToolButton { id: optbutton;  source: "options.svg";  }
            ToolButton { id: mapbutton;  source: "map.svg";      onClicked: showPage(mapselectsrc)  }
            ToolButton { id: catbutton;  source: "category.svg"; onClicked: showPage(catselectsrc) }
            ToolButton { id: tripbutton; source: "trip.svg";     onClicked: showPage(tripselectsrc) }
            ToolButton { id: tstbutton;  source: "flag.svg";     onClicked: showPage(tstselectsrc)  }

            // Opts => ?
            // Map  => Meta | Local | All  | Cat | Trip
            // Cat  => Meta | Wpt   | Rte  | Trk | Map
            // Trip => Meta | Wpt   | Rte  | Trk | Map

            hasRightButton: true
            ToolButton {
                id: quitbutton
                source: "exit.svg"
                onClicked: exitClient();
                onRepeat:  exitClientAndServer();
                interval: 5000;
            }
        }

        Component.onCompleted: {
            toolbar.hide = settings.getProperty("toolbar_hide",false)
        }
    }

    DashBoard {
        id: dashboard
        y: hide? (landscape? 360*h: 640*h) : (landscape? 360*h-height: 640*h-height)
        property int animationDuration: 300
        pageStack: root.pageStack

        Behavior on y {
            enabled: root.animate
            NumberAnimation { easing.type: Easing.InOutQuart; duration: dashboard.animationDuration }
        }

        function toggleHide() {
            hide = hide? false: true;
            settings.setProperty("dash_hide", hide)
        }

        Component.onCompleted: {
            dashboard.hide = settings.getProperty("dash_hide",false)
        }
    }

    ToolButton {
        x: landscape? 260*w : 150*w
        y: landscape? 327*h : 607*h
        height: 60*h
        width: 60*w
        id: hidebutton;
        source: "export.svg";
        onClicked: {
            root.animate = true;
            dashboard.toggleHide();
            toolbar.toggleHide();
            root.animate = false;
        }
    }

    ToolButton {
        x: landscape? 640*w - width*1.1 : 360*w - width*1.1
        y: 60*h
        height: 40*h
        width: 40*w
        id: zoominbutton;
        source: "zoom-in.svg";
        onClicked: map.zoomIn()
    }

    ToolButton {
        x: landscape? 640*w - width*1.1 : 360*w - width*1.1
        y: 115*h
        height: 40*h
        width: 40*w
        id: zoomoutbutton;
        source: "zoom-out.svg";
        onClicked: map.zoomOut()
    }
}
