import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "qrc:/js/helpers.js" as Helpers

Item {
    id: root
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

    AltitudeModel {
        id: altmodel
    }

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: altmodel.reset()
    }

    Image {
        source: "/images/speed10.svg"
        anchors.fill: parent
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
            text: Helpers.toFixed(altmodel.average,1)
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
        Text {
            id: bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 2
            text: Helpers.toFixed(altmodel.max,1)
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
    }

    Image {
        source: "/images/shorthand.svg"
        anchors.fill: parent
        transform: Rotation {
            id: shorthand
            origin.x: width/2
            origin.y: height/2
            angle: -180 + altmodel.current/10000*360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }

    Image {
        source: "/images/longhand.svg"
        anchors.fill: parent
        transform: Rotation {
            id: longhand
            origin.x: width/2
            origin.y: height/2
            angle: -180 + ((altmodel.current/1000) % 1000) *360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
