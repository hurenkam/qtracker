import QtQuick 1.0

Rectangle {
    id: root
    color: "black"
    width: parent.width
    height: 50

    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: Qt.lighter(activePalette.light)
        }
        GradientStop {
            position:  1.0
            color: Qt.lighter(activePalette.dark)
        }
    }

    property Item tools: null

    onWidthChanged:        if (tools) tools.layoutChildren(root)
    onToolsChanged:        if (tools) tools.layoutChildren(root)
    Component.onCompleted: if (tools) tools.layoutChildren(root)
}
