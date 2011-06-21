import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root
    property alias mapid: database.mapid

    //signal editRefpoint(int index)
    signal addRefpoint()
    signal editRefpoint(int index, int mappt, string name, double latitude, double longitude, int x, int y)

    function saveRefpoint(index, mapid, name, lat, lon, x, y) {
        if (index < 0) {
            database.append( { mapid: mapid, name: name, latitude: lat, longitude: lon, x: x, y: y } )
        } else {
            database.set(index, { mapid: mapid, name: name, latitude: lat, longitude: lon, x: x, y: y } )
        }
        update()
    }

    Component {
        id: delegate
        OptionTextItem { text: "" }
    }
/*
    Database {
        id: database
        table: "mappoints"
        onCountChanged: lst.update()

        Component.onCompleted: {
            console.log("RefpointList.database.onCompleted")
            database.exec("CREATE TABLE IF NOT EXISTS refpoints (refid INTEGER PRIMARY KEY, mapid INTEGER, name TEXT, latitude REAL, longitude REAL, x REAL, y REAL);")
        }
    }
*/
    Refpoints {
        id: database
        onFilterChanged: update();
    }

    items: content

    DynamicItemModel {
        id: content
        name: "RefpointList"
        //OptionTextItem { text: "<new>"; button: true; }
    }

    function update() {
        var item = null;
        content.clear();
        item = delegate.createObject(null)
        item.text = "<new>"
        item.button = true;
        content.append(item)
        console.log("refpointlist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("refpointlist item ",database.get(i,"name"))
            item = delegate.createObject(null)
            item.text = database.get(i,"mappt") + " " + database.get(i,"name")
            item.button = true
            content.append(item)
        }
        root.layout()
    }

    onClicked: {
        console.log("RefpointList.onClicked",index,text);
        if (index === 0) {
            addRefpoint()
        } else {
            var item = database.get(index-1)
            editRefpoint(index-1,item.mappt,item.name,item.latitude,item.longitude,item.x,item.y)
        }
    }

    Component.onCompleted: update()
}
