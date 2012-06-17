import QtQuick 1.0
import QtMobility.location 1.1
import "../components"
import "../model"

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

    PositionSource {
        id: gps
        updateInterval: 500
        active: true
    }
/*
    SpeedModel {
        id: speedmodel
        interval: 500
    }

    DistanceModel {
        id: distancemodel
        interval: 1000
    }
*/
    function updateSpeed(cur,avg,min,max)
    {
        //speed.value = cur
        minimum.value = min
        maximum.value = max
        average.value = avg
    }
    function updateDistance(cur,mon)
    {
        distance.value = cur
        monitor.value = mon
    }
    property list<QtObject> values: [
        Item { id: speed;    property double value: gps.position.speedValid? gps.position.speed : 0 },
        Item { id: average;  property double value: 0 },
        Item { id: minimum;  property double value: 0 },
        Item { id: maximum;  property double value: 0 },
        Item { id: distance; property double value: 0 },
        Item { id: monitor;  property double value: 0 }
    ]
    property double analogvalue: values[analogindex].value? values[analogindex].value : -180
    property double topvalue:    values[topindex].value?    values[topindex].value    : 0
    property double bottomvalue: values[bottomindex].value? values[bottomindex].value : 0

    function reset() {
        speedmodel.reset()
        distancemodel.reset()
    }

    property int count: 5
    onCountChanged: setCount(count)
    function setCount(c) {
        speedmodel.setCount(c)
    }
    property real hysteresis: 25.0
    onHysteresisChanged: setHysteresis(hysteresis)
    function setHysteresis(h) {
        distancemodel.setHysteresis(h)
    }

    Image {
        //source: if (analog.value < 10) "speed10.png"; else "speed200.png"
        source: (analogvalue && (analogvalue>10))? "speed200.png" : "speed10.png"
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
        source: "speedneedle.png"
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
