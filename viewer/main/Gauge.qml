import QtQuick 1.0
import "../components"

Item {
    id: root
    property Item gauge
    property Item main

    signal clicked()

    onGaugeChanged: {
        console.log("Gauge.onGaugeChanged")
        gauge.parent = root
    }

    Connections {
        target: gauge
        onClicked: clicked()
    }
}
