import QtQuick 1.0
import QmlTrackerModel 1.0

Rectangle {
    id: root
    anchors.margins: 5
    border { width: 1; color: Qt.darker(activePalette.dark) }
    smooth: true
    radius: 10
    property int viewid: -1
    x:      parent.gaugeX(viewid)
    y:      parent.gaugeY(viewid)
    width:  parent.gaugeW(viewid)
    height: parent.gaugeH(viewid)
    Behavior on x      { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on y      { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on width  { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on height { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }

    signal menuClicked()
    signal waypointClicked()
    signal routeClicked()
    signal trackClicked()
    signal loadClicked()
    signal saveClicked()

    signal singleTap()
    signal doubleTap()
    signal longTap()

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

        Flickable {
            id: flickable
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

            MouseHandler {
                id: mouse
                onSingleTap: root.singleTap()
                onDoubleTap: root.doubleTap()
                onLongTap:   root.longTap()
            }
        }
        function bx(i) {
            if (i>=0) return i*55 -60; else return map.width + i * 55 +10
        }

        function by(i) {
            if (i>=0) return i*55 -60; else return map.height + i * 55 +10
        }

        MapButton { id: menu;     source: "options.svg";  x: map.bx(1);  y: map.by(-1); onClicked: root.menuCliced()      }
        MapButton { id: exit;     source: "exit.svg";     x: map.bx(-1); y: map.by(-1); onClicked: Qt.quit()              }
        MapButton { id: zoomin;   source: "zoom-in.svg";  x: map.bx(-1); y: map.by(1);  onClicked: map.zoomIn()           }
        MapButton { id: zoomout;  source: "zoom-out.svg"; x: map.bx(-1); y: map.by(2);  onClicked: map.zoomOut()          }
        MapButton { id: waypoint; source: "flag.svg";     x: map.bx(1);  y: map.by(1);  onClicked: root.waypointClicked() }
        MapButton { id: route;    source: "route.svg";    x: map.bx(2);  y: map.by(1);  onClicked: root.routeClicked()    }
        MapButton { id: track;    source: "hiker.svg";    x: map.bx(3);  y: map.by(1);  onClicked: root.trackClicked()    }
        MapButton { id: load;     source: "import.svg";   x: map.bx(4);  y: map.by(1);  onClicked: root.loadClicked()     }
        MapButton { id: save;     source: "export.svg";   x: map.bx(5);  y: map.by(1);  onClicked: root.saveClicked()     }
    }
}
