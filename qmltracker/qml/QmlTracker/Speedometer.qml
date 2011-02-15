import QtQuick 1.0

Item {
    id: root
    property double value: model.speed();
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
        onSpeedChanged: { value = model.speed(); top.text = model.distance().toString(); }
    }

    Image {
        source: if (root.value < 10) "speed10.svg"; else "speed200.svg"
        width: parent.width
        height: parent.height
    }

    Rectangle {
        y: parent.height * 0.8
        height: parent.height * 0.12
        color: activePalette.dark
        width: parent.width/2.5
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: top
            anchors.horizontalCenter: parent.horizontalCenter
            text: model.distance()
            color: "white"
            font.bold: true; font.pixelSize: parent.height/1.25
            style: Text.Raised; styleColor: "black"
        }
    }

    Image {
        source: "speedneedle.svg"
        width: parent.width
        height: parent.height
        transform: Rotation {
            id: needle
            origin.x: width/2
            origin.y: height/2
            angle: if (root.value < 10) root.value/10*360 -180; else root.value/200*360 -180
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                }
            }
        }
    }
}
