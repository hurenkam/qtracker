import QtQuick 1.0
import QmlTrackerExtensions 1.0

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

    SatelliteModel {
        id: satellitemodel
    }

    Repeater {
        id: positions
        anchors.margins: 5
        anchors.fill: parent
        smooth: true
        clip:  true
        focus: true

        Component {
            id: satellitedelegate
            //Image {
                //source: inuse? "/images/locator_green.svg" : "/images/locator_red.svg"
            Rectangle {
                color: inuse? "#00ff00" : Qt.rgba(strength/255.0,strength/255.0,0,1)
                width: root.width/20
                height: root.height/20
                radius: root.width/40
                x: (root.width-root.width/15)/2
                y: (root.height-root.height/15)/2

                transform: [
                    Translate {
                        y: elevation * root.height / 180.0
                    },
                    Rotation {
                        origin.x: width/2
                        origin.y: height/2
                        angle: azimuth - 180
                    }
                ]
            }
        }

        model: satellitemodel
        delegate: satellitedelegate
    }
}
