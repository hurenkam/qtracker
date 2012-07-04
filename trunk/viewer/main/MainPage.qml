import QtQuick 1.0
import "../components"
import "../model"

Page {
    id: root
    imageSource: "../components/options-bg.png"
    property bool animate: false
    property string url: "http://127.0.0.1:8280/RPC2"

    function exitServer() {
        rpc.exit();
    }
    function exitClient() {
        console.log("exitClient()")
        Qt.quit();
    }
    function exitClientAndServer() {
        console.log("exitClientAndServer()")
        root.exitServer();
        exittimer.start();
    }

    Timer {
        id: exittimer
        interval: 300
        running: false
        repeat: false
        onTriggered: {
            root.exitClient()
        }
    }

    Item {
        id: activePalette
        property color light: "#505050"
        property color dark: "#101010"
    }

    XmlRpc {
        id: rpc;
        url: root.url

        function exit() {
            rpc.call("exit",[])
        }

        function startTrack(id,interval) {
            rpc.call("start",[id,interval])
        }

        function stopTrack() {
            rpc.call("stop",[])
        }
    }

    Settings      { id: settings }

    Component { id: catselectsrc;  CategoryEditPage  { id: catPage;  } }
    //Component { id: tstselectsrc;  ImExportPage      { id: tstPage;  } }
    Component { id: mapselectsrc;
        MapEditPage {
            id: mapPage;
            mapid: map.mapid
            onMapSelected: map.loadMap(mapid);
        }
    }
    //Component {
    //    id: tripselectsrc;
        TripEditPage {
            id: tripPage;
        }
    //}

    Loader {
        id: pageloader
        onItemChanged: {
            console.log("MainPage.pageloader.onItemChanged:",pageloader.item.id)
        }
    }

    function showPage(src) {
        pageloader.sourceComponent = src
        //pageloader.item.setupMapArguments(map.mapid,map.mapname,map.maplat,map.maplon,0.0,map.mapx,map.mapy)
        pageStack.push(pageloader.item)
    }
/*
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
            source: map.state == "scrolling"? "../Images/locator_red.png" : "../Images/locator_green.png"
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
*/
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
            ToolButton { id: optbutton;  source: "../components/options.png";  }
            ToolButton { id: mapbutton;  source: "map.png";      onClicked: showPage(mapselectsrc)  }
            ToolButton { id: catbutton;  source: "category.png"; onClicked: showPage(catselectsrc) }
            ToolButton { id: tripbutton; source: "trip.png";     onClicked: pageStack.push(tripPage) }//showPage(tripselectsrc) }
            //ToolButton { id: tstbutton;  source: "flag.png";     onClicked: showPage(tstselectsrc)  }

            // Opts => ?
            // Map  => Meta | Local | All  | Cat | Trip
            // Cat  => Meta | Wpt   | Rte  | Trk | Map
            // Trip => Meta | Wpt   | Rte  | Trk | Map

            hasRightButton: true
            ToolButton {
                id: quitbutton
                source: "../components/exit.png"
                onClicked: exitClient();
                onRepeat:  exitClientAndServer();
                interval: 5000;
            }
        }

        Component.onCompleted: {
            toolbar.hide = settings.getProperty("toolbar_hide",false)
        }
    }

    TrackModel    { id: trackmodel;    url: root.url; interval: 1000 }
    LocationModel { id: locationmodel; url: root.url; interval: 1000 }
    CourseModel   { id: coursemodel;   url: root.url; interval: 1000 }
    TimeModel     {
        id: timemodel;
        interval: 1000
        onDataChanged: {
            tripPage.updateTime(current,elapsed,monitor)
        }
    }

    TripModel     {
        id: tripmodel;
        interval: 1000
        onDataChanged: {
            tripPage.updateTrip(key,name,state)
        }
    }

    SpeedModel    {
        id: speedmodel
        interval: 1000
        onDataChanged: {
            dashboard.updateSpeed(current,average,minimum,maximum)
            tripPage.updateSpeed(current,average,minimum,maximum)
        }
    }
    DistanceModel {
        id: distancemodel
        interval: 1000
        onDataChanged: {
            dashboard.updateDistance(current,monitor)
            tripPage.updateDistance(current,monitor)
        }
    }
    AltitudeModel {
        id: altitudemodel
        interval: 1000
        onDataChanged: {
            dashboard.updateAltitude(current,average,minimum,maximum,ascent,descent)
            tripPage.updateAltitude(current,average,minimum,maximum,ascent,descent)
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
        source: "export.png";
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
        source: "zoom-in.png";
        onClicked: map.zoomIn()
    }

    ToolButton {
        x: landscape? 640*w - width*1.1 : 360*w - width*1.1
        y: 115*h
        height: 40*h
        width: 40*w
        id: zoomoutbutton;
        source: "zoom-out.png";
        onClicked: map.zoomOut()
    }

    //Component.onCompleted: update.restart()
}
