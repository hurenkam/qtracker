import QtQuick 1.0
import "../Components"

Item {
    id: root
    property Item gauge
    property Item main

    signal clicked()

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
    }

    onGaugeChanged: {
        console.log("Gauge.onGaugeChanged")
        gauge.parent = root
    }
}
