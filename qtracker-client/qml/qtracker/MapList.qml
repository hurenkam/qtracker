import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "qrc:/js/filesystem.js" as FileSystem

OptionList {
    id: root
    x: 0; width: parent.width;
    anchors.top: edit.bottom;
    anchors.bottom: parent.bottom;
    title: "List"

    signal mapSelected(int index, string baseName, string fileName)

    FolderListModel {
        id: maplist
        //folder: (client.platform==0) ? "file:///e:/data/qtracker/maps/" : "file:///c:/data/qtracker/maps/"
        //folder: "file:///c:/data/qtracker/maps/"
        folder: "file:///e:/data/qtracker/maps/"
        nameFilters: ["*.jpg"]
    }

    Component {
        id: delegate
        OptionTextItem { text: ""; button: true }
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
        console.log("content contains",content.count(),"items",content.get(0),content.get(0).text)
        lst.layout()
    }

    Component.onCompleted: update()

    onClicked: {
        root.mapSelected(index,text,maplist.folder + text + ".jpg");
    }
}
