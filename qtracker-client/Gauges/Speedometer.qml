import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

Item {
    id: root
    property int viewid: -1
    property int analogindex: -1
    property int topindex:    -1
    property int bottomindex: -1

    signal clicked()
    signal options()

    function toFixed(num,count) {
        var s = num.toFixed(count).toString();
        return s;
    }

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: root.reset()
        onDoubleTap: root.options()
    }

    function update() {
        analogindex = settings.getProperty("speed_analog",0)
        topindex =    settings.getProperty("speed_top",3)
        bottomindex = settings.getProperty("speed_bottom",4)
    }

    Component.onCompleted: update()

    property list<QtObject> values: [
        ValueSpaceSubscriber { id: speedcur; path: "/server/speed/current"     },
        ValueSpaceSubscriber { id: speedmin; path: "/server/speed/min"         },
        ValueSpaceSubscriber { id: speedmax; path: "/server/speed/max"         },
        ValueSpaceSubscriber { id: speedavg; path: "/server/speed/average"     },
        ValueSpaceSubscriber { id: distance; path: "/server/location/distance" },
        ValueSpaceSubscriber { id: monitor;  path: "/server/monitor/distance"  }
    ]
    property double analogvalue: values[analogindex].value? values[analogindex].value : -180
    property double topvalue:    values[topindex].value?    values[topindex].value    : 0
    property double bottomvalue: values[bottomindex].value? values[bottomindex].value : 0


    function reset() {
        console.log("speedometer.reset()")
        var cmd = { "class": "speed", "method": "reset", "args": [] }
        client.sendCommand(cmd);

        cmd = { "class": "location", "method": "reset", "args": [] }
        client.sendCommand(cmd);
    }
    property int count: 5
    onCountChanged: setCount(count)
    function setCount(c) {
        console.log("speedometer.setCount(",c,")")
        var cmd = { "class": "speed", "method": "setcount", "args": [ c ] }
        client.sendCommand(cmd);
    }
    property real hysteresis: 25.0
    onHysteresisChanged: setHysteresis(hysteresis)
    function setHysteresis(h) {
        console.log("speedometer.setHysteresis(",h,")")
        var cmd = { "class": "location", "method": "sethysteresis", "args": [ h ] }
        client.sendCommand(cmd);
    }

    Image {
        //source: if (analog.value < 10) "speed10.svg"; else "speed200.svg"
        source: (analogvalue && (analogvalue>10))? "../Images/speed200.svg" : "../Images/speed10.svg"
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
            //id: top
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 2
            //text: root.toFixed(top.value,1)
            text: root.toFixed(topvalue,1)
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
        Text {
            //id: bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 2
            //text: root.toFixed(bottom.value,1)
            text: root.toFixed(bottomvalue,1)
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
    }

    Image {
        source: "../Images/speedneedle.svg"
        width: parent.width
        height: parent.height
        transform: Rotation {
            id: needle
            origin.x: width/2
            origin.y: height/2
            //angle: if (analog.value < 10) analog.value/10*360 -180; else analog.value/200*360 -180
            angle: analogvalue<10? analogvalue/10*360 -180: analogvalue/200*360 -180
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
