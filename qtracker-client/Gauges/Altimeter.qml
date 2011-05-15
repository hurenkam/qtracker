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

    Image {
        source: "speed10.svg"
        anchors.fill: parent
    }

    function update() {
        analogindex = settings.getProperty("altimeter_analog",3)
        topindex =    settings.getProperty("altimeter_top",1)
        bottomindex = settings.getProperty("altimeter_bottom",2)
    }

    Component.onCompleted: update()

    property list<QtObject> values: [
        ValueSpaceSubscriber { id: current; path: "/server/altitude/current" },
        ValueSpaceSubscriber { id: min;     path: "/server/altitude/min"     },
        ValueSpaceSubscriber { id: max;     path: "/server/altitude/max"     },
        ValueSpaceSubscriber { id: average; path: "/server/altitude/average" },
        ValueSpaceSubscriber { id: ascent;  path: "/server/altitude/ascent"  },
        ValueSpaceSubscriber { id: descent; path: "/server/altitude/descent" }
    ]
    property double analogvalue: values[analogindex].value? values[analogindex].value : 0
    property double topvalue:    values[topindex].value?    values[topindex].value    : 0
    property double bottomvalue: values[bottomindex].value? values[bottomindex].value : 0

    function reset() {
        console.log("altimeter.reset()")
        var cmd = { "class": "altitude", "method": "reset", "args": [] }
        client.sendCommand(cmd);
    }

    property int count: 10
    onCountChanged: setCount(count)
    function setCount(c) {
        console.log("altimeter.setCount(",c,")")
        var cmd = { "class": "altitude", "method": "setcount", "args": [ c ] }
        client.sendCommand(cmd);
    }

    property real hysteresis: 25.0
    onHysteresisChanged: setHysteresis(hysteresis)
    function setHysteresis(h) {
        console.log("altimeter.setHysteresis(",h,")")
        var cmd = { "class": "altitude", "method": "sethysteresis", "args": [ h ] }
        client.sendCommand(cmd);
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
            text: root.toFixed(topvalue,1)
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
        Text {
            id: bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 2
            text: root.toFixed(bottomvalue,1)
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
            //angle: -180 + current.value/10000 * 360
            angle: -180 + analogvalue/10000 * 360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
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
            //angle: -180 + current.value/1000 * 360
            angle: -180 + analogvalue/1000 * 360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
