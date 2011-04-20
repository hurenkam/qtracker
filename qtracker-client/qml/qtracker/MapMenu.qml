import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "qrc:/js/filesystem.js" as FileSystem

Page {
    id: root
    imageSource: "qrc:/images/options-bg.png"

    signal mapSelected(string fileName)

    function cancel() {
        pageStack.pop();
    }

    OptionHeader {
        id: hdr
        x: 0; y:0; width: parent.width; height: 50
        text: "Map"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    OptionBox {
        id: edit
        title: "Options"
        x: 0; width: parent.width;
        anchors.top: hdr.bottom;
        items: optionitems

        DynamicItemModel {
            id: optionitems
            name: "optionitems"

            VisualItemModel {
                OptionItem { text: "Name" }
                OptionItem { text: "Calibration" }
                OptionItem { text: "Datum" }
                Component.onCompleted: {
                    for (var i=0; i<count; i++) {
                        optionitems.append(children[i])
                    }
                }
            }
        }
    }

    OptionList {
        id: lst
        x: 0; width: parent.width;
        anchors.top: edit.bottom;
        anchors.bottom: parent.bottom;
        title: "List"

        FolderListModel {
            id: maplist
            //folder: (client.platform==0) ? "file:///e:/data/qtracker/maps/" : "file:///c:/data/qtracker/maps/"
            //folder: "file:///c:/data/qtracker/maps/"
            folder: "file:///e:/data/qtracker/maps/"
            nameFilters: ["*.jpg"]
        }

        Component {
            id: delegate
            OptionItem { text: "" }
        }

        items: content

        DynamicItemModel {
            id: content
            name: "MapList"
        }

        function update() {
            var item = null;
            content.clear()
            console.log("maplist contains",maplist.count,"items")
            for (var i=0; i<maplist.count; i++) {
                console.log("maplist item ",maplist.get(i,"fileName"))
                item = delegate.createObject(null)
                item.text = FileSystem.base(maplist.get(i,"fileName"))
                content.append(item)
            }
            console.log("content contains",content.count,"items",content.get(0),content.get(0).text)
            lst.layout()
        }

        Component.onCompleted: update()

        onClicked: {
            root.mapSelected(maplist.folder + text + ".jpg");
            pageStack.pop();
        }
    }
}
