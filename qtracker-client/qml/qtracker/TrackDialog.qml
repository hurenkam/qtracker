import QtQuick 1.0
//import "qrc:/colibri"
import com.nokia.symbian 1.0
//import Qt.labs.components 1.0

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
