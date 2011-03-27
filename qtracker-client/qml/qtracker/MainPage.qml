import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

Page {
    id: root

    function exitServer() {
        console.log("exitServer()")
        //client.stopServer();
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

    TrackDialog    { id: trkDialog }
    WaypointDialog { id: wptDialog }
    RouteDialog    { id: rteDialog }
    MapDialog      { id: mapDialog }

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
            source: map.state == "scrolling"? "/images/locator_red.svg" : "/images/locator_green.svg"
            width: sourceSize.width/2
            height: sourceSize.height/2
            x: (parent.width-width)/2
            y: (parent.height-height)/2
        }
    }
    Connections {
        target: mapDialog
        onMapSelected: {
            //state="normal"
            map.loadMap(fileName)
        }
    }

    ToolBar {
        id: toolbar
        Behavior on height { NumberAnimation { easing.type: Easing.InOutQuart; duration: 400 } }

        function toggleHide() {
            hide = hide? false: true;
        }

        tools: ToolBarLayout {

            ToolButton { id: mapbutton; source: "qrc:/images/options.svg"; onClicked: root.pageStack.push(mapDialog); }
            ToolButton { id: wptbutton; source: "qrc:/images/flag.svg";    onClicked: root.pageStack.push(wptDialog); }
            ToolButton { id: rtebutton; source: "qrc:/images/route.svg";   onClicked: root.pageStack.push(rteDialog); }
            ToolButton { id: trkbutton; source: "qrc:/images/hiker.svg";   onClicked: root.pageStack.push(trkDialog); }
            ToolButton { id: iobutton;  source: "qrc:/images/export.svg";  }

            hasRightButton: true
            ToolButton {
                id: quitbutton
                source: "qrc:/images/exit.svg"
                onClicked: exitClient();
                onRepeat:  exitClientAndServer();
                interval: 5000;
            }
        }
    }

    DashBoard {
        id: dashboard
        //anchors.bottom: root.bottom
        y: hide? (landscape? 360*h: 640*h) : (landscape? 360*h-height: 640*h-height)
        Behavior on y { NumberAnimation { easing.type: Easing.InOutQuart; duration: 400 } }

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
        source: "qrc:/images/export.svg";
        onClicked: { dashboard.toggleHide(); toolbar.toggleHide(); }
    }
}
