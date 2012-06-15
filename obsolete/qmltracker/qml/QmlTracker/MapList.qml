import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "qrc:/js/filesystem.js" as FileSystem

Rectangle {
    id: root
    anchors.margins: 5
    clip:  true
    property string mapname: ""
    property alias index: maplist.currentIndex

    color: activePalette.dark
    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: activePalette.light
        }
        GradientStop {
            position:  1.0
            color: activePalette.dark
        }
    }

    ListView {
        id: maplist
        anchors.margins: 5
        anchors.fill: parent
        smooth: true
        clip:  true

        FolderListModel {
            id: folderModel
            //folder: "file:///c:/data/qmltracker/maps/"
            folder: "file:///e:/data/qmltracker/maps/"
            nameFilters: ["*.jpg"]
        }

        Component {
            id: fileDelegate
            Text {
                text: FileSystem.base(fileName); color: "white"; font.pixelSize: 24
                MouseArea {
                    id: mouseArea
                    anchors.fill:  parent
                    onClicked: {
                        root.mapname = folderModel.folder + fileName
                        maplist.currentIndex = index
                    }
                }
            }
        }

        model: folderModel
        delegate: fileDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5; width: parent.width }
        focus: true
    }
}
