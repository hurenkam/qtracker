import QtQuick 1.0

Rectangle {
    id: root
    color: "black"
    width: parent.width/w
    height: hide? 0: 50
    property bool hide: false
    //scale: w
    transform: Scale { origin.x: 0; origin.y: 0; xScale: w; yScale: h }

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

    function setTools(newtools) {
        tools.hideChildren()
        if (tools) tools.parent = null
        tools = newtools
        tools.layoutChildren(root)
    }

    onWidthChanged:        if (tools) tools.layoutChildren(root)
    Component.onCompleted: if (tools) tools.layoutChildren(root)
}
