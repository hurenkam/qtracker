import QtQuick 1.0
import QmlTrackerExtensions 1.0

Item {
    id: root
    PositionModel { id: positionmodel; }
    CompassModel { id: compassmodel; }

    //property int compass:     model.compassLevel()
    property int compass:     compassmodel.quality
    property int battery:     model.batteryLevel()
    //property int horizontal:  model.horizontalLevel()
    //property int vertical:    model.verticalLevel()
    property int horizontal:  positionmodel.horizontal
    property int vertical:    positionmodel.vertical
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

    Connections {
        target: model
        //onPositionChanged: { horizontal = model.horizontalLevel(); vertical = model.verticalLevel() }
        //onHeadingChanged:  { compass = model.compassLevel() }
        onTimeChanged:     { battery = model.batteryLevel() }
    }

    Image {
        source: "/images/level.svg"
        anchors.fill: parent
    }

    Image {
        source: "/images/levelhand.svg"
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
        source: "/images/levelhand.svg"
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
        source: "/images/levelhand.svg"
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
        source: "/images/levelhand.svg"
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
}
