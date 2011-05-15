import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

Item {
    id: root
    property bool headingup:       true
    property int viewid:           -1
    property int sourceindex:      -1
    property int orientationindex: -1

    signal clicked()
    signal options()

    function update() {
        sourceindex =      settings.getProperty("compass_source",0)
        orientationindex = settings.getProperty("compass_orientation",0)
    }

    Component.onCompleted: update()

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: root.reset()
        onDoubleTap: root.options()
    }

    property list<QtObject> values: [
        ValueSpaceSubscriber { path: "/server/compass/azimuth"  },
        ValueSpaceSubscriber { path: "/server/location/heading" },
        ValueSpaceSubscriber { path: "/server/monitor/bearing"  }
    ]
    property double azimuth: (sourceindex==0)? (values[0].value? values[0].value: 0) : (values[1].value? values[1].value : 0)
    property double bearing: values[2].value? values[2].value : 0
    property bool northup: (orientationindex==1)

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
            //angle: (bearing.value && azimuth.value)? bearing.value - azimuth.value : 0
            angle: northup? bearing : bearing - azimuth
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
            //angle: azimuth.value? 360 - azimuth.value : 0
            angle: northup? 0 : 360 - azimuth
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
            angle: northup? azimuth : 0
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
