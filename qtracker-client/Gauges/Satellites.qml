import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

Item {
    id: root
    property int viewid: -1

    signal clicked()
    signal options()

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        //onLongTap: root.reset()
        onDoubleTap: root.options()
    }

    Image {
        id: background
        source: "../Images/satview.svg"
        anchors.fill: parent
    }

    ValueSpaceSubscriber {
        id: inview;
        path: "/server/satellite/inview"
    }

    ValueSpaceSubscriber {
        id: inuse;
        path: "/server/satellite/inuse"
    }

    Component {
        id: satelliteinview
        Rectangle {
            color: Qt.rgba(1,modelData.strength/32.0,0,1)
            width: root.width/20
            height: root.height/20
            radius: root.width/40
            anchors.centerIn: parent

            transform: [
                Translate {
                    y: modelData.elevation * root.height / 180.0
                },
                Rotation {
                    origin.x: width/2
                    origin.y: height/2
                    angle: modelData.azimuth - 180
                }
            ]

        }
    }

    Component {
        id: satelliteinuse
        Rectangle {
            color: "#00ff00"
            width: root.width/20
            height: root.height/20
            radius: root.width/40
            anchors.centerIn: parent

            transform: [
                Translate {
                    y: modelData.elevation * root.height / 180.0
                },
                Rotation {
                    origin.x: width/2
                    origin.y: height/2
                    angle: modelData.azimuth - 180
                }
            ]

        }
    }

    Repeater {
        id: positionsinview
        model: inview.value
        delegate: satelliteinview
    }

    Repeater {
        id: positionsinuse
        model: inuse.value
        delegate: satelliteinuse
    }
}
