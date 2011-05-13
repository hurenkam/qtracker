import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

Item {
    id: root
    property bool headingup: true
    property int viewid: -1
    //x:      parent.gaugeX(viewid)
    //y:      parent.gaugeY(viewid)
    //width:  parent.gaugeW(viewid)
    //height: parent.gaugeH(viewid)
    //Behavior on x      { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    //Behavior on y      { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    //Behavior on width  { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    //Behavior on height { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }

    signal clicked()
    signal options()

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: root.reset()
        onDoubleTap: root.options()
    }

    ValueSpaceSubscriber { id: azimuth; path: "/server/compass/azimuth" }
    ValueSpaceSubscriber { id: bearing; path: "/server/monitor/bearing" }

    function reset() {
        console.log("compass.reset()")
        var cmd = { "class": "compass", "method": "reset", "args": [] }
        client.sendCommand(cmd);
    }

    Image {
        source: "compassring.svg"
        anchors.fill: parent
        transform: Rotation {
            id: ring
            origin.x: width/2
            origin.y: height/2
            //angle: 360 - azimuth.value
            angle: (bearing.value && azimuth.value)? bearing.value - azimuth.value : 0
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
        source: "compass.svg"
        anchors.fill: parent
        transform: Rotation {
            id: compass
            origin.x: width/2
            origin.y: height/2
            //angle: 360 - azimuth.value
            angle: azimuth.value? 360 - azimuth.value : 0
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
        source: "compassneedle.svg"
        anchors.fill: parent
        transform: Rotation {
            id: needle
            origin.x: width/2
            origin.y: height/2
            angle: 0
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }
}
