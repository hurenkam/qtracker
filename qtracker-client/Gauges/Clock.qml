import QtQuick 1.0
import QmlTrackerExtensions 1.0
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

    TimeModel {
        id: timemodel
    }

    property list<QtObject> values: [
        Item { property date value: timemodel.current; property string text: Qt.formatDateTime(value,"hh:mm:ss"); },
        Item { property date value: timemodel.elapsed; property string text: Qt.formatDateTime(value,"hh:mm:ss"); },
        Item { property date value: timemodel.monitor; property string text: Qt.formatDateTime(value,"hh:mm:ss"); }
    ]
    function reset() {
        timemodel.reset()
    }

    Image {
        source: "../Images/clock.png";
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
        source: "../Images/shorthand.png"
        anchors.fill: parent
        transform: Rotation {
            id: shorthand
            origin.x: width/2
            origin.y: height/2
            angle: values[analogindex].value.getHours()*360/12 +  values[analogindex].value.getMinutes()/2
/*
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
*/
        }
    }
    Image {
        source: "../Images/longhand.png"
        anchors.fill: parent
        transform: Rotation {
            id: longhand
            origin.x: width/2
            origin.y: height/2
            angle: values[analogindex].value.getMinutes()*360/60
/*
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
*/
        }
    }
    Image {
        source: "../Images/secondhand.png"
        anchors.fill: parent
        transform: Rotation {
            id: secondhand
            origin.x: width/2
            origin.y: height/2
            angle: values[analogindex].value.getSeconds()*360/60

/* Disabled for now due to performance reasons
   http://code.google.com/p/qtracker/issues/detail?id=11

            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
*/
        }
    }
}
