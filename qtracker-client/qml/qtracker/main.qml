import QtQuick 1.0
//import com.nokia.symbian 1.0
//import Qt.labs.components 1.0
import QmlTrackerExtensions 1.0

Rectangle {
    id: main
    SystemPalette { id: activePalette }
    color: activePalette.dark
    state: "normal"

    Layout      { id: view;        visible: false }
    MapDialog   { id: mapdialog;   visible: false }
    TrackDialog { id: trackdialog; visible: false }

    function hideAll() {
        view.visible = false
        mapdialog.visible = false
        trackdialog.visible = false
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
        },
        State {
            name: "trackdialog"
            StateChangeScript { script: main.hideAll() }
            PropertyChanges { target: trackdialog; visible: true }
        }
    ]

    Connections { target: view.mapview; onMenuClicked:  state="mapdialog"   }
    Connections { target: view.mapview; onTrackClicked: state="trackdialog" }
    Connections { target: mapdialog;    onCancel:       state="normal"      }
    Connections { target: trackdialog;  onCancel:       state="normal"      }

    Connections {
        target: mapdialog
        onMapSelected: {
            state="normal"
            view.mapview.selectMap(fileName)
        }
    }
}
