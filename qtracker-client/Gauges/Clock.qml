import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

Item {
    id: root
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

    ValueSpaceSubscriber {
        id: analog;
        path: "/server/time/current"
        property int hour:   (value==undefined)? 0 : value.hour
        property int minute: (value==undefined)? 0 : value.minute
        property int second: (value==undefined)? 0 : value.second
        property string text: hour.toString() + ":" + minute.toString() + "." + second.toString()
    }
    ValueSpaceSubscriber {
        id: top;
        path: "/server/time/trip"
        property int hour:   (value==undefined)? 0 : value.hour
        property int minute: (value==undefined)? 0 : value.minute
        property int second: (value==undefined)? 0 : value.second
        property string text: hour.toString() + ":" + minute.toString() + "." + second.toString()
    }
    ValueSpaceSubscriber {
        id: bottom;
        path: "/server/monitor/time"
        property int hour:   (value==undefined)? 0 : value.hour
        property int minute: (value==undefined)? 0 : value.minute
        property int second: (value==undefined)? 0 : value.second
        property string text: hour.toString() + ":" + minute.toString() + "." + second.toString()
    }

    function reset() {
        console.log("clock.reset()")
        var cmd = { "class": "time", "method": "reset", "args": [] }
        client.sendCommand(cmd);
    }

    Image {
        source: "clock.svg";
        anchors.fill: parent
    }

    Rectangle {
        y: parent.height * 0.7
        height: parent.height * 0.18
        color: activePalette.dark
        width: parent.width/3.5
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: toptext
            anchors.horizontalCenter: parent.horizontalCenter
            text: top.text
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
        Text {
            y: parent.height/2
            id: bottomtext
            anchors.horizontalCenter: parent.horizontalCenter
            text: bottom.text
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
    }

    Image {
        source: "shorthand.svg"
        anchors.fill: parent
        transform: Rotation {
            id: shorthand
            origin.x: width/2
            origin.y: height/2
            angle:  analog.hour*360/12 + analog.minute/2
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
        source: "longhand.svg"
        anchors.fill: parent
        transform: Rotation {
            id: longhand
            origin.x: width/2
            origin.y: height/2
            angle: analog.minute*360/60
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
        source: "secondhand.svg"
        anchors.fill: parent
        transform: Rotation {
            id: secondhand
            origin.x: width/2
            origin.y: height/2
            angle: analog.second*360/60
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
