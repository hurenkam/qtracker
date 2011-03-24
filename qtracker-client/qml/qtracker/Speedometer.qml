import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
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

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: root.reset()
    }

    ValueSpaceSubscriber { id: analog; path: "/server/speed/current"     }
    ValueSpaceSubscriber { id: top;    path: "/server/location/distance" }
    ValueSpaceSubscriber { id: bottom; path: "/server/monitor/distance"  }
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
        source: if (analog.value < 10) "/images/speed10.svg"; else "/images/speed200.svg"
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
            text: Helpers.toFixed(top.value,1)
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
        Text {
            //id: bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 2
            text: Helpers.toFixed(bottom.value,1)
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
            angle: if (analog.value < 10) analog.value/10*360 -180; else analog.value/200*360 -180
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
