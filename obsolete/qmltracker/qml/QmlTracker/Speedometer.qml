import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "qrc:/js/helpers.js" as Helpers

Item {
    id: root
    property double value: speedmodel.current
    property int viewid: -1
    x:      parent.gaugeX(viewid)
    y:      parent.gaugeY(viewid)
    width:  parent.gaugeW(viewid)
    height: parent.gaugeH(viewid)
    Behavior on x      { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on y      { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on width  { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on height { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }

    signal clicked()

    SpeedModel {
        id: speedmodel
    }

    MonitorModel {
        id:monitormodel
    }

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: speedmodel.reset()
    }

    Image {
        source: if (root.value < 10) "/images/speed10.svg"; else "/images/speed200.svg"
        width: parent.width
        height: parent.height
    }

    Rectangle {
        y: parent.height * 0.75
        height: parent.height * 0.16
        color: activePalette.dark
        width: parent.width/4
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: top
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 2
            text: Helpers.toFixed(speedmodel.distance,1)
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
        Text {
            id: bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 2
            text: Helpers.toFixed(monitormodel.distance,1)
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
    }

    Image {
        source: "/images/speedneedle.svg"
        width: parent.width
        height: parent.height
        transform: Rotation {
            id: needle
            origin.x: width/2
            origin.y: height/2
            angle: if (root.value < 10) root.value/10*360 -180; else root.value/200*360 -180
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
