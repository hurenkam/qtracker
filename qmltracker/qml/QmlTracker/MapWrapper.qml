import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "qrc:/js/filesystem.js" as FileSystem

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

    signal singleTap()
    signal doubleTap()
    signal longTap()

    signal menuClicked()
    signal waypointClicked()
    signal routeClicked()
    signal trackClicked()
    signal loadClicked()
    signal saveClicked()

    function selectMap(m) {
        console.log("selecting map",m)
        map.loadMap(m)
    }

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
    MapView {
        id: map
        onSingleTap: root.singleTap()
        onDoubleTap: root.doubleTap()
        onLongTap:   root.longTap()
    }
    Image {
        id: locator
        source: "/images/locator_red.svg"
        width: locator.sourceSize.width/2
        height: locator.sourceSize.height/2
        x: (parent.width-width)/2
        y: (parent.height-height)/2
    }

    function bx(i) { if (i>=0) return (i-1)*55+5; else return width + i * 55  }
    function by(i) { if (i>=0) return (i-1)*55+5; else return height + i * 55 }
    function bw(i) { return i*55 - 5 }
    function bh(i) { return bw(i) - 1 }

    Rectangle {
        id: toolbar;
        x: bx(1); y: by(1); width: bw(5); height: bh(1)
        anchors.margins: 5
        border { width: 1; color: Qt.darker(activePalette.dark) }
        smooth: true
        radius: 23
        opacity: 0.7
    }
    Rectangle {
        id: zoombar;
        x: bx(-1); y: by(1); width: bw(1); height: bh(2)
        anchors.margins: 5
        border { width: 1; color: Qt.darker(activePalette.dark) }
        smooth: true
        radius: 23
        opacity: 0.7
    }
    Rectangle {
        id: status;
        x: bx(1); y: by(-1); width: bw(5); height: bh(1)
        anchors.margins: 5
        border { width: 1; color: Qt.darker(activePalette.dark) }
        smooth: true
        radius: 23
        opacity: 0.7

        PositionModel {
            id: positionmodel
        }
        Text {
            id: gpspos
            anchors.leftMargin: bx(2)+5
            anchors.topMargin:  5;
            anchors.top: parent.top
            anchors.left: parent.left
            text: positionmodel.position
            color: "black"
            font.bold: true; font.pixelSize: 15
            style: Text.Raised; styleColor: "black"
        }
        Text {
            id: mappos
            anchors.leftMargin: bx(2)+5
            anchors.bottomMargin:  5;
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            text: map.position()
            color: "black"
            font.bold: true; font.pixelSize: 15
            style: Text.Raised; styleColor: "black"
        }
        Text {
            id: mapstatus
            anchors.leftMargin: bx(5)-5
            anchors.bottomMargin:  5;
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            text: map.status == Image.Error? "Error" : map.status == Image.Ready? "Ready" : "Other"
            color: "black"
            font.bold: true; font.pixelSize: 15
            style: Text.Raised; styleColor: "black"
        }
    }

    MapButton { id: waypoint; source: "/images/flag.svg";      x: bx(1);  y: by(1);  onClicked: root.waypointClicked() }
    MapButton { id: route;    source: "/images/route.svg";     x: bx(2);  y: by(1);  onClicked: root.routeClicked()    }
    MapButton { id: track;    source: "/images/hiker.svg";     x: bx(3);  y: by(1);  onClicked: root.trackClicked()    }
    MapButton { id: load;     source: "/images/import.svg";    x: bx(4);  y: by(1);  onClicked: root.loadClicked()     }
    MapButton { id: save;     source: "/images/export.svg";    x: bx(5);  y: by(1);  onClicked: root.saveClicked()     }
    MapButton { id: menu;     source: "/images/options.svg";   x: bx(1);  y: by(-1); onClicked: root.menuClicked()      }
    MapButton { id: exit;     source: "/images/exit.svg";      x: bx(-1); y: by(-1); onClicked: Qt.quit()              }
    MapButton { id: zoomin;   source: "/images/zoom-in.svg";   x: bx(-1); y: by(1);  onRepeat: map.zoomIn(); interval: 100 }
    MapButton { id: zoomout;  source: "/images/zoom-out.svg";  x: bx(-1); y: by(2);  onRepeat: map.zoomOut(); interval: 100 }

    Connections { target: model; onPositionChanged: gpspos.text = model.position() }
    Connections { target: map;   onPositionChanged: mappos.text = map.position()   }
}
