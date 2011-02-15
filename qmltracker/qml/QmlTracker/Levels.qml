import QtQuick 1.0

Item {
    id: root
    width: parent.width; height: parent.height
    property int compass:     model.compassLevel()
    property int battery:     model.batteryLevel()
    property int horizontal:  model.horizontalLevel()
    property int vertical:    model.verticalLevel()

    signal clicked()

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }

    Connections {
        target: model
        onPositionChanged: { horizontal = model.horizontalLevel(); vertical = model.verticalLevel() }
        onHeadingChanged:  { compass = model.compassLevel() }
        onTimeChanged:     { battery = model.batteryLevel() }
    }

    Image {
        source: "level.svg"
        width: parent.width
        height: parent.height
    }

    Image {
        source: "levelhand.svg"
        width: parent.width
        height: parent.height*0.55
        transform: Rotation {
            origin.x: width/2
            origin.y: height/2
            angle: ((battery*0.7) -35) % 360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }

    Image {
        source: "levelhand.svg"
        width: parent.width
        height: parent.height*0.55
        transform: Rotation {
            origin.x: width/2
            origin.y: height/2
            angle: ((compass*0.7) -125) % 360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }

    Image {
        source: "levelhand.svg"
        width: parent.width
        height: parent.height*0.55
        transform: Rotation {
            origin.x: width/2
            origin.y: height/2
            angle: (((100-vertical)*0.7) +55) % 360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }

    Image {
        source: "levelhand.svg"
        width: parent.width
        height: parent.height*0.55
        transform: Rotation {
            origin.x: width/2
            origin.y: height/2
            angle: (((100-horizontal)*0.7) +145) % 360
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }

/*    Image {
        source: "leveltop.svg"
        width: parent.width
        height: parent.height
    }*/
}
