import QtQuick 1.0

Rectangle {
    id: button

    property string text: "Button"

    signal clicked

    width: buttonLabel.width + 20; height: buttonLabel.height + 5
    border { width: 1; color: Qt.darker(activePalette.button) }
    smooth: true
    radius: 8
    anchors.margins: 5

    gradient: Gradient {
        GradientStop {
            position:  0.0
            color: {
                if (mouseArea.pressed)
                    return activePalette.dark
                else
                    return activePalette.light
            }
        }
        GradientStop {
            position: 1.0
            color: activePalette.button
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill:  parent
        onClicked: button.clicked();
    }

    Text {
        id: buttonLabel
        anchors.centerIn:  button
        color: activePalette.buttonText
        text: button.text
    }
}
