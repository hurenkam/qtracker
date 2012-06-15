import QtQuick 1.0
import QmlTrackerExtensions 1.0

Item {
    id: root
    property real heading: compassmodel.current
    property real bearing: compassmodel.bearing
    property bool headingup: true
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

    CompassModel {
        id: compassmodel
    }

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: clockmodel.reset()
    }

    Image {
        source: "/images/compassring.svg"
        anchors.fill: parent
        transform: Rotation {
            id: ring
            origin.x: width/2
            origin.y: height/2
            angle: if (root.headingup) (root.bearing - root.heading) % 360; else root.bearing
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }
    Image {
        source: "/images/compass.svg"
        anchors.fill: parent
        transform: Rotation {
            id: compass
            origin.x: width/2
            origin.y: height/2
            angle: if (root.headingup) (360 - root.heading) % 360; else 0
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }
    Image {
        source: "/images/compassneedle.svg"
        anchors.fill: parent
        transform: Rotation {
            id: needle
            origin.x: width/2
            origin.y: height/2
            angle: if (root.headingup) 0; else root.heading
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
