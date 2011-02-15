import QtQuick 1.0

Item {
    id: root
    property double value: model.altitude()
    width: parent.width; height: parent.height

    signal clicked()

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }

    Connections {
        target: model
        onAltitudeChanged: value = model.altitude()
    }

    Image {
        source: "speed10.svg"
        width: parent.width
        height: parent.height
    }

    Image {
        source: "shorthand.svg"
        width: parent.width
        height: parent.height
        transform: Rotation {
            id: shorthand
            origin.x: width/2
            origin.y: height/2
            angle: -180 + root.value/10000*360
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
        width: parent.width
        height: parent.height
        transform: Rotation {
            id: longhand
            origin.x: width/2
            origin.y: height/2
            angle: -180 + ((root.value/1000) % 1000) *360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
