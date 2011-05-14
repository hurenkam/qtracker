import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

Item {
    id: root
    property int viewid: -1
    property int analogindex: -1
    property int topindex:    -1
    property int bottomindex: -1

    Settings { id: settings }

    function update() {
        analogindex = settings.getProperty("clock_analog",0)
        topindex =    settings.getProperty("clock_top",0)
        bottomindex = settings.getProperty("clock_bottom",1)
    }

    Component.onCompleted: update()

    signal clicked()
    signal options()

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: root.reset()
        onDoubleTap: root.options()
    }

    property list<QtObject> values: [
        ValueSpaceSubscriber {
            id: currenttime
            path: "/server/time/current"
            property int hour:   value? value.hour   : 0
            property int minute: value? value.minute : 0
            property int second: value? value.second : 0
            property string text: hour.toString() + ":" + minute.toString() + "." + second.toString()
            //onTextChanged: console.log("current:   ", text)
        },
        ValueSpaceSubscriber {
            id: triptime
            path: "/server/time/trip"
            property int hour:   value? value.hour   : 0
            property int minute: value? value.minute : 0
            property int second: value? value.second : 0
            property string text: hour.toString() + ":" + minute.toString() + "." + second.toString()
            //onTextChanged: console.log("trip:      ", text)
        },
        ValueSpaceSubscriber {
            id: monitortime
            path: "/server/monitor/time"
            property int hour:   value? value.hour   : 0
            property int minute: value? value.minute : 0
            property int second: value? value.second : 0
            property string text: hour.toString() + ":" + minute.toString() + "." + second.toString()
            //onTextChanged: console.log("monitor:   ", text)
        }
    ]

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
            text: values[topindex].text
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
        Text {
            y: parent.height/2
            id: bottomtext
            anchors.horizontalCenter: parent.horizontalCenter
            text: values[bottomindex].text
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
            angle: values[analogindex].hour*360/12 +  values[analogindex].minute/2
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
            angle: values[analogindex].minute*360/60
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
            angle: values[analogindex].second*360/60
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
