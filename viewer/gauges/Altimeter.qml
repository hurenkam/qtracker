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

    Image {
        source: "speed10.png"
        anchors.fill: parent
    }

    function update() {
        analogindex = settings.getProperty("altimeter_analog",3)
        topindex =    settings.getProperty("altimeter_top",1)
        bottomindex = settings.getProperty("altimeter_bottom",2)
    }

    Component.onCompleted: update()

    PositionSource {
        id: gps
        updateInterval: 1000
        active: true
        onPositionChanged: if (gps.position.altitudeValid) current.value = gps.position.coordinate.altitude
    }

    AltitudeModel {
        id: server
    }

    property list<QtObject> values: [
        Item { id: current; property double value: server.current },
        Item { id: min;     property double value: server.minimum },
        Item { id: max;     property double value: server.maximum },
        Item { id: average; property double value: server.average },
        Item { id: ascent;  property double value: server.ascent  },
        Item { id: descent; property double value: server.descent }
    ]
    property double analogvalue: values[analogindex].value? values[analogindex].value : 0
    property double topvalue:    values[topindex].value?    values[topindex].value    : 0
    property double bottomvalue: values[bottomindex].value? values[bottomindex].value : 0

    function reset() {
        server.reset()
    }

    property int count: 10
    onCountChanged: setCount(count)
    function setCount(c) {
        server.setCount(c)
    }

    property real hysteresis: 25.0
    onHysteresisChanged: setHysteresis(hysteresis)
    function setHysteresis(h) {
        server.setHysteresis(h)
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
        source: "shorthand.png"
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
        source: "longhand.png"
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
