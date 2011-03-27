import QtQuick 1.0
//import "qrc:/colibri"
//import com.nokia.symbian 1.0
//import Qt.labs.components 1.0

Page {
    id: root
    state: "list"
    signal trackSelected(int trackId)
    signal trackStarted(int trackId)
    signal trackStopped(int trackId)

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }
    function add() {
        state="edit"
    }
    function list() {
        state="list"
    }

    ToolBar {
        id: toolbar

        tools: ToolBarLayout {
            hasRightButton: true
            ToolButton { id: backbutton;    source: "qrc:/images/import.svg";  onClicked: cancel();  }
            ToolButton { id: listbutton;    source: "qrc:/images/options.svg"; onClicked: list();    }
            ToolButton { id: addbutton;     source: "qrc:/images/zoom-in.svg"; onClicked: add();     }
            ToolButton { id: confirmbutton; source: "qrc:/images/visible.svg"; onClicked: confirm(); visible: false }
        }
    }

    TrackList {
        id:tracklist
        width: parent.width
        //height: parent.height-30
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        index: -1
    }

    states: [
        State {
            name: "list";
            PropertyChanges { target: confirmbutton; visible: false }
            PropertyChanges { target: tracklist;     visible: true  }
        },
        State {
            name: "edit";
            PropertyChanges { target: confirmbutton; visible: true  }
            PropertyChanges { target: tracklist;     visible: false }
        }
    ]

}
/*
Page {
    signal cancel()
    signal confirm()
    signal add()
    signal edit()
    signal options()
    property alias index: tracklist.index

    ToolBar {
        id: tools
        width: parent.width
        anchors.top: parent.top

        Row {
            ToolButton { iconSource: "qrc:/images/export.svg";  onClicked: cancel();  }
            ToolButton { iconSource: "qrc:/images/visible.svg"; onClicked: confirm(); }
            ToolButton { iconSource: "qrc:/images/zoom-in.svg"; onClicked: add();     }
            ToolButton { iconSource: "qrc:/images/edit.svg";    onClicked: edit();    }
            ToolButton { iconSource: "qrc:/images/options.svg"; onClicked: options(); }
        }
    }

    TrackList {
        id:tracklist
        width: parent.width
        //height: parent.height-30
        anchors.top: tools.bottom
        anchors.bottom: parent.bottom
        index: -1
    }
}
*/
