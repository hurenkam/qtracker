import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../Components"

OptionList {
    id: root
    property int platform: 0

    signal mapSelected(int mapid, string name)
    signal importMap()

    Settings {
        id: settings
    }

    GpxFile {
        id: gpxfile
        property int mapid: -1

        onFileNameChanged: {
            console.log("GpxFile.onFileNameChanged(): ",fileName)
            reset()
            parseGpx()
        }

        onRefPoint: {
            console.log("GpxFile.onRefpointFound()",mapid,refpt.x,refpt.y,refpt.latitude,refpt.longitude);
            refpoints.append({ mapid: mapid, latitude: refpt.latitude, longitude: refpt.longitude, x: refpt.x, y: refpt.y})
        }

        onResolution: console.log("GpxFile.onResolutionFound()",resolution.width,resolution.height);
    }

    Refpoints {
        id: refpoints
    }

    function addMap(name,folder) {
        console.log("Maps.importMap(",name,",",folder,")")

        gpxfile.mapid = database.append({ name: name, filename: folder+name+".jpg" } )
        gpxfile.fileName = folder+name+".xml"
        //gpxfile.fileName = "c:\\data\\qtracker\\maps\\51a_oisterwijk.xml"

    }

    Maps {
        id: database
        onCountChanged: root.update()
        onDataChanged: root.update()
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
        content.clear();
        console.log("maplist contains",database.count,"items")
        item = delegate.createObject(null)
        item.text = "<import>"
        item.button = true;
        content.append(item)
        for (var i=0; i<database.count; i++) {
            console.log("maplist item ",database.get(i).mapid,database.get(i).name)
            item = delegate.createObject(null)
            item.text = database.get(i).mapid + " " + database.get(i).name
            content.append(item)
        }
        root.layout()
    }

    Component.onCompleted: {
        root.update()
    }

    onClicked: {
        if (index === 0)
        {
            console.log("MapList.onClicked()",index)
            root.importMap()
        }
        else
        {
            console.log("MapList.onClicked()",index,database.get(index-1).mapid)
            root.mapSelected(database.get(index-1).mapid,database.get(index-1).name);
        }
    }
}
