import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root

    signal editRefpoint(int index)

    function saveRefpoint(index, mapid, name, lat, lon, x, y) {
        if (index < 0) {
            database.append( { mapid: mapid, name: name, latitude: lat, longitude: lon, x: x, y: y } )
        } else {
            database.set(index, { mapid: mapid, name: name, latitude: lat, longitude: lon, x: x, y: y } )
        }
    }

    Component {
        id: delegate
        OptionTextItem { text: "" }
    }

    Database {
        id: database
        table: "refpoints"
        onCountChanged: lst.update()

        Component.onCompleted: {
            console.log("RefpointList.database.onCompleted")
            database.exec("CREATE TABLE IF NOT EXISTS refpoints (refid INTEGER PRIMARY KEY, mapid INTEGER, name TEXT, latitude REAL, longitude REAL, x REAL, y REAL);")
        }
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
            item.text = database.get(i,"refpointid") + " " + database.get(i,"name")
            content.append(item)
            lst.layout()
        }
    }

    onClicked: {
        console.log("RefpointList.onClicked",index,text);
        editRefpoint(index-1)
    }

    Component.onCompleted: update()
}
