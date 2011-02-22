import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "qrc:/js/filesystem.js" as FileSystem

Rectangle {
    id: root
    anchors.margins: 5
    clip:  true
    property string mapname: ""
    property alias index: maplist.currentIndex

    //border { width: 1; color: Qt.darker(activePalette.dark) }
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
            //folder: "file:///c:/workspace/QmlTracker/data/maps/"
            folder: "file:///e:/data/qtracker/maps/"
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
/*
                        console.log("map:        ", folderModel.folder + fileName)
                        resolution.source= folderModel.folder + fileName
                        for (var i=0; i<resolution.count; i++) {
                            console.log("resolution: ",resolution.get(i).width,",",resolution.get(i).height)
                        }
                        refpoint.source= folderModel.folder + fileName
                        for (var i=0; i<refpoint.count; i++) {
                            console.log("refpoint:   ",refpoint.get(i).x,refpoint.get(i).y,refpoint.get(i).lat,refpoint.get(i).lon)
                        }*/
                    }
                }
            }
        }

        model: folderModel
        delegate: fileDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5; width: parent.width }
        focus: true
    }
/*
    XmlListModel {
        id: resolution
        query: "/map/resolution"
        XmlRole { name: "width";  query: "@width/number()" }
        XmlRole { name: "height"; query: "@height/number()" }
    }
    XmlListModel {
        id: refpoint
        query: "/map/refpoint"
        XmlRole { name: "lat"; query: "@lat/number()" }
        XmlRole { name: "lon"; query: "@lon/number()" }
        XmlRole { name: "x";   query: "@x/number()"   }
        XmlRole { name: "y";   query: "@y/number()"   }
    }
*/
}
