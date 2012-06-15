import QtQuick 1.0
//import QtMobility.publishsubscribe 1.1
import "../Components"

TabOptionPage {
    id: root
    title: "Satellite Options"
    showheader: false
    signal optionsChanged()
    property bool landscape: (width>height)
    property Item mapview

    background: Rectangle {
        id: gauge
        objectName: "gauge"
        anchors.fill: parent
        property double margin: root.landscape? (height-radius)/2 : (width-radius)/2
        property double radius: root.landscape? height*0.9 : width*0.9

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.darker(activePalette.light)
            }
            GradientStop {
                position:  1.0
                color: Qt.darker(activePalette.dark)
            }
        }

        Satellites {
            id: sats;
            objectName: "sats"
            x: gauge.margin
            y: gauge.margin
            width: gauge.radius
            height: gauge.radius
        }

        ToolButton {
            id: leftbutton
            x: 10; y:10
            width: root.width/7
            height: width

            bgcolor: "black"
            source: "../Images/backc.png";
            onClicked: root.cancel();
        }

        ToolButton {
            id: rightbutton

            x: root.width - 10 -width; y:10
            width: root.width/7
            height: width

            source: "../Images/confirmc.png";
            bgcolor: "black"
            //onClicked: root.optionsChanged();

            onClicked: {
                console.log("CompassOptionsPage.onConfirm")
                optionsChanged()
                pageStack.pop()
            }

        }
    }
}
