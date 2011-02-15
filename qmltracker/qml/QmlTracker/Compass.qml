import QtQuick 1.0
//import TrackerModel 1.0

Item {
    id: root
    property real heading: model.heading()
    property real bearing: model.bearing()
    property bool headingup: true
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
        onHeadingChanged: heading = model.heading()
    }

    function headingChanged() {
        heading = model.heading()
    }

    function bearingChanged() {
        bearing = model.beading()
    }

    Image {
        source: "compassring.svg"
        anchors.fill: parent
        //width: parent.width
        //height: parent.height
        transform: Rotation {
            id: ring
            origin.x: width/2
            origin.y: height/2
            angle: if (root.headingup) (root.bearing - root.heading) % 360; else root.bearing
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
        source: "compass.svg"
        anchors.fill: parent
        //width: parent.width
        //height: parent.height
        transform: Rotation {
            id: compass
            origin.x: width/2
            origin.y: height/2
            angle: if (root.headingup) (360 - root.heading) % 360; else 0
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
        source: "compassneedle.svg"
        anchors.fill: parent
        //width: parent.width
        //height: parent.height
        transform: Rotation {
            id: needle
            origin.x: width/2
            origin.y: height/2
            angle: if (root.headingup) 0; else root.heading
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
