import QtQuick 1.0
import QmlTrackerExtensions 1.0
//import "file:///c:/workspace/colibri"

Rectangle {
    id: main
    SystemPalette { id: activePalette }
    color: activePalette.dark
    state: "normal"

    Layout    { id: view;      visible: false }
    MapDialog { id: mapdialog; visible: false }

    function hideAll() {
        view.visible = false
        mapdialog.visible = false
    }

    states: [
        State { name: "normal"
            StateChangeScript { script: main.hideAll() }
            PropertyChanges { target: view; visible: true }
        },
        State {
            name: "mapdialog"
            StateChangeScript { script: main.hideAll() }
            PropertyChanges { target: mapdialog; visible: true }
        }
    ]

    Connections { target: view.mapview; onMenuClicked: state="mapdialog" }
    Connections { target: mapdialog;    onCancel:      state="normal"    }

    Connections {
        target: mapdialog
        onMapSelected: {
            state="normal"
            view.mapview.selectMap(fileName)
        }
    }
}
