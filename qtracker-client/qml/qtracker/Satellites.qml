import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

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

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }

    Image {
        id: background
        source: "/images/satview.svg"
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