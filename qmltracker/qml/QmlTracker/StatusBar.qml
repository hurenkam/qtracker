import QtQuick 1.0
import QmlTrackerModel 1.0

Rectangle {
    id: root
    color: activePalette.light
    anchors.margins: 5
    radius: 5

    Connections {
        target: model
        onPositionChanged: {
            position.text = model.position()
        }
    }

    Text {
        id: position
        x: root.width/10
        anchors.verticalCenter: root.verticalCenter
        color: activePalette.buttonText
        text: model.position()
    }
}
