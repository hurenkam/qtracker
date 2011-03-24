import QtQuick 1.0
import "qrc:/colibri"

CLTab {
    id: root
    tabHeight: 24
    //tabWidth: 60
    anchors.fill: parent
    anchors.margins: 5
    visible: false

    signal mapSelected(string fileName)
    signal cancel()

    Item {
        property string title: "Local"
        anchors.fill:  parent

        CLButton {
            id: localselect
            text: "Select"
            height: 30
            width: parent.width/3
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            fontSize: 8
            onClicked: root.cancel()
        }
        CLButton {
            id: localcancel
            text: "Cancel"
            height: 30
            width: parent.width/3
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            fontSize: 8
            onClicked: root.cancel()
        }
    }
    Item {
        property string title: "All"
        anchors.fill:  parent
        anchors.topMargin: 12

        MapList {
            id: maplist
            width: parent.width
            height: parent.height-30
            index: -1
        }

        CLButton {
            id: allselect
            text: "Select"
            height: 30
            width: parent.width/3
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            fontSize: 8
            onClicked: root.mapSelected(maplist.mapname)
        }
        CLButton {
            id: allcancel
            text: "Cancel"
            height: 30
            width: parent.width/3
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            fontSize: 8
            onClicked: root.cancel()
        }
    }
    Item {
        property string title: "New"
        anchors.fill:  parent
        CLButton {
            id: newsave
            text: "Save"
            height: 30
            width: parent.width/3
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            fontSize: 8
            onClicked: root.cancel()
        }
        CLButton {
            id: newcancel
            text: "Cancel"
            height: 30
            width: parent.width/3
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            fontSize: 8
            onClicked: root.cancel()
        }
    }
    Item {
        property string title: "Datum"
        anchors.fill:  parent
        CLButton {
            id: datumselect
            text: "Select"
            height: 30
            width: parent.width/3
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            fontSize: 8
            onClicked: root.cancel()
        }
        CLButton {
            id: datumcancel
            text: "Cancel"
            height: 30
            width: parent.width/3
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            fontSize: 8
            onClicked: root.cancel()
        }
    }
}
