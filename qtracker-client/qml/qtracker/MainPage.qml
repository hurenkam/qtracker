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
        anchors.top: toolbar.bottom;
        anchors.bottom: parent.bottom;
        width: parent.width;
        onSingleTap: root.singleTap()
        onDoubleTap: root.doubleTap()
        onLongTap:   root.longTap()
    }
    Image {
        id: locator
        source: map.state == "scrolling"? "/images/locator_red.svg" : "/images/locator_green.svg"
        width: locator.sourceSize.width/2
        height: locator.sourceSize.height/2
        x: (parent.width-width)/2
        y: (parent.height-height)/2
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

    Image {
        source: landscape? "qrc:/images/dash-bg-landscape.ico": "qrc:/images/dash-bg-portrait.ico"
        anchors.bottom: parent.bottom
        height: sourceSize.height*w
        width: sourceSize.width*w
    }
    Clock {
        id:     clock
        x:      landscape? 5*w: 0*w
        y:      landscape? parent.height-80*w: parent.height-200*w
        width:  80*w
        height: 80*w
    }
    Compass {
        id:     compass
        x:      landscape?               440*w:                80*w
        y:      landscape? parent.height-200*w: parent.height-275*w
        width:  198*w
        height: 198*w
    }
    Satellites {
        id:     sats
        x:      landscape? 365*w: 280*w
        y:      landscape? parent.height-85*w: parent.height-200*w
        width:  80*w
        height: 80*w
    }
    Speedometer {
        id:     speed
        x:      0
        y:      landscape? parent.height-205*w: parent.height-120*w
        width:  120*w
        height: 120*w
    }
    Altimeter {
        id:     altimeter
        x:      landscape? 90*w: 240*w
        y:      parent.height-120*w
        width:  120*w
        height: 120*w
    }

    Image {
        source: landscape? "qrc:/images/dash-fg-landscape.ico": "qrc:/images/dash-fg-portrait.ico"
        anchors.bottom: parent.bottom
        height: sourceSize.height*w
        width: sourceSize.width*w
    }
}
