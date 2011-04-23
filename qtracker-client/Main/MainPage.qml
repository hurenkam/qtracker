import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"
import "../Map"
import "../Waypoint"
import "../Route"
import "../Track"

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

    TrackSelectionPage    { id: trkPage }
    WaypointSelectionPage { id: wptPage }
    RouteSelectionPage    { id: rtePage }
    MapEditPage           { id: mapPage }
    ImExportPage          { id: options }

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
    }

    Connections {
        //target: mapDialog
        target: mapPage
        onMapSelected: {
            //state="normal"
            map.loadMap(fileName)
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
        }

        tools: ToolBarLayout {
            ToolButton { id: mapbutton; source: "options.svg"; onClicked: root.pageStack.push(mapPage); }
            ToolButton { id: wptbutton; source: "flag.svg";    onClicked: root.pageStack.push(wptPage); }
            ToolButton { id: rtebutton; source: "route.svg";   onClicked: root.pageStack.push(rtePage); }
            ToolButton { id: trkbutton; source: "hiker.svg";   onClicked: root.pageStack.push(trkPage); }
            ToolButton { id: iobutton;  source: "export.svg";  onClicked: root.pageStack.push(options); }

            hasRightButton: true
            ToolButton {
                id: quitbutton
                source: "exit.svg"
                onClicked: exitClient();
                onRepeat:  exitClientAndServer();
                interval: 5000;
            }
        }
    }

    DashBoard {
        id: dashboard
        y: hide? (landscape? 360*h: 640*h) : (landscape? 360*h-height: 640*h-height)
        property int animationDuration: 300
        Behavior on y {
            enabled: root.animate
            NumberAnimation { easing.type: Easing.InOutQuart; duration: dashboard.animationDuration }
        }

        function toggleHide() {
            hide = hide? false: true;
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
