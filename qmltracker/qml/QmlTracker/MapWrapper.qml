import QtQuick 1.0
import QmlTrackerModel 1.0

Rectangle {
    id: mapwrapper
    anchors.margins: 5
    border { width: 1; color: Qt.darker(activePalette.dark) }
    smooth: true
    radius: 10

    signal menuClicked()
    signal waypointClicked()
    signal routeClicked()
    signal trackClicked()
    signal loadClicked()
    signal saveClicked()

    signal singleTap()
    signal doubleTap()
    signal longTap()
    //signal drag(int x, int y)

    color: activePalette.dark

    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: activePalette.light
        }
        GradientStop {
            position:  1.0
            color: activePalette.dark
        }
    }

    Map {
        id: map
        anchors.margins: 10
        anchors.fill: parent
        //property MouseEvent previous: {}
        property int prevx: 0
        property int prevy: 0

        states: [
            State { name: "pending" },
            State { name: "initial" },
            State { name: "second"  },
            State { name: "long"    }/*,
            State { name: "drag"    }*/
        ]

        function handleSingleTapTimeout() {
            //console.log("handleSingleTapTimeout, state: ", map.state)
            if (map.state == "pending") {
                console.log("singleTap")
                map.state = ""
                singleTap()
            }
        }

        function handleLongTapTimeout() {
            //console.log("handleLongTapTimeout, state: ", map.state)
            if ((map.state == "initial") || (map.state == "second")) {
                state = "long"
            }
        }

        function handleMousePressed(event) {
            //console.log("handleMousePressedEvent, state: ", map.state)
            prevx = event.x
            prevy = event.y
            if (map.state == "") {
                singletap.start();
                longtap.start();
                map.state = "initial";
            }
            if (map.state == "pending") {
                singletap.stop();
                state = "second";
            }
        }

        function handleMouseReleased(event) {
            //console.log("handleMouseReleasedEvent, state: ", map.state)
            if (map.state == "initial") {
                longtap.stop()
                singletap.start()
                map.state = "pending"
            }
            if (map.state == "second") {
                console.log("doubleTap")
                longtap.stop()
                doubleTap()
                map.state = "";
            }
            if (map.state == "long") {
                console.log("longTap")
                longTap();
                map.state = "";
            }
            /*
            if (map.state == "drag") {
                var dx = prevx - event.x
                var dy = prevy - event.y
                prevx = event.x
                prevy = event.y
                map.pan(dx,dy)
                map.state = "";
            }*/
        }

        function handleMouseMove(event) {
            var dx = prevx - event.x
            var dy = prevy - event.y

            if ((map.state == "drag")
             || (map.state == "initial")
             || (map.state == "second")
             || (map.state == "long")) {
                map.state = "drag"
                prevx = event.x
                prevy = event.y
                map.pan(dx,dy)
            }
        }

        function handleMouseCanceled() {
            //console.log("handleMouseCanceledEvent, state: ", map.state)
            singletap.stop()
            longtap.stop()
            map.state=""
        }

        Timer {
            id: singletap; interval: 200; running: false; repeat: false;
            onTriggered: map.handleSingleTapTimeout()
        }

        Timer {
            id: longtap;   interval: 800; running: false; repeat: false;
            onTriggered: map.handleLongTapTimeout()
        }
/*
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onPressed: map.handleMousePressed(mouse)
            onReleased: map.handleMouseReleased(mouse)
            onPositionChanged: map.handleMouseMove(mouse)
        }
*/

        Flickable {
            id: flickable
            //anchors.fill: parent.fill
            anchors.left: parent.left
            anchors.right:  parent.right
            anchors.bottom:  parent.bottom
            anchors.top: parent.top
            contentX: map.mapx
            contentY: map.mapy
            contentWidth: map.mapw + map.width
            contentHeight: map.maph + map.height
            onContentXChanged: map.moveTo(contentX, contentY)
            onContentYChanged: map.moveTo(contentX, contentY)
            clip: true

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onPressed: map.handleMousePressed(mouse)
                onReleased: map.handleMouseReleased(mouse)
                onCanceled: map.handleMouseCanceled()
            }
        }



        function bx(i) {
            if (i>=0) return i*55 -60; else return map.width + i * 55 +10
        }

        function by(i) {
            if (i>=0) return i*55 -60; else return map.height + i * 55 +10
        }

        MapButton { id: menu;     source: "options.svg";  x: map.bx(1);  y: map.by(-1) }
        MapButton { id: exit;     source: "exit.svg";     x: map.bx(-1); y: map.by(-1) }
        MapButton { id: zoomin;   source: "zoom-in.svg";  x: map.bx(-1); y: map.by(1)  }
        MapButton { id: zoomout;  source: "zoom-out.svg"; x: map.bx(-1); y: map.by(2)  }
        MapButton { id: waypoint; source: "flag.svg";     x: map.bx(1);  y: map.by(1)  }
        MapButton { id: route;    source: "route.svg";    x: map.bx(2);  y: map.by(1)  }
        MapButton { id: track;    source: "hiker.svg";    x: map.bx(3);  y: map.by(1)  }
        MapButton { id: load;     source: "import.svg";   x: map.bx(4);  y: map.by(1)  }
        MapButton { id: save;     source: "export.svg";   x: map.bx(5);  y: map.by(1)  }

        Connections { target: menu;     onClicked: mapwrapper.menuClicked() }
        Connections { target: exit;     onClicked: Qt.quit() }
        Connections { target: zoomin;   onClicked: map.zoomIn() }
        Connections { target: zoomout;  onClicked: map.zoomOut() }
        Connections { target: waypoint; onClicked: mapwrapper.waypointClicked() }
        Connections { target: route;    onClicked: mapwrapper.routeClicked() }
        Connections { target: track;    onClicked: mapwrapper.trackClicked() }
        Connections { target: load;     onClicked: mapwrapper.loadClicked() }
        Connections { target: save;     onClicked: mapwrapper.saveClicked() }
    }
}
