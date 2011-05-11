import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../Components"

OptionList {
    id: root
    property int platform: 0

    signal mapSelected(int index, string baseName, string fileName)

    Settings {
        id: settings
    }

    FolderListModel {
        id: maplist
        //folder: (platform==0) ? "file:///e:/data/qtracker/maps/" : "file:///c:/data/qtracker/maps/"
        //folder: "file:///c:/data/qtracker/maps/"
        //folder: "file:///e:/data/qtracker/maps/"
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

    function base(filename) {
        var txt = String(filename);
        var p1 = txt.lastIndexOf('/');
        var p2 = txt.lastIndexOf('.');
        return txt.slice(p1+1,p2);
    }

    function update() {
        var item = null;
        content.clear()
        console.log("maplist contains",maplist.count,"items")
        for (var i=0; i<maplist.count; i++) {
            console.log("maplist item ",maplist.get(i,"fileName"))
            item = delegate.createObject(null)
            item.text = base(maplist.get(i,"fileName"))
            content.append(item)
        }
        console.log("content contains",content.count(),"items",content.get(0),content.get(0).text)
        lst.layout()
    }

    //Component.onCompleted: update()
    Component.onCompleted: {
        var mapdir = settings.getProperty("map_directory",(maplist.platform==0) ? "file:///e:/data/qtracker/maps/" : "file:///c:/data/qtracker/maps/")
        maplist.folder = mapdir
        root.update()
    }

    onClicked: {
        root.mapSelected(index,text,maplist.folder + text + ".jpg");
    }
}
