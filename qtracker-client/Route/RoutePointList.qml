import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root
    property int routeid: -1
    onRouteidChanged: console.log("RoutePointList.routeid",root.routeid)

    signal routepointSelected(int index, int routeid, string name, real latitude, real longitude, real altitude)

    function saveRoutepoint(index, name, latitude, longitude, altitude) {
        console.log("saveRoutepoint",index, root.routeid, name, latitude, longitude, altitude)
        if (index < 0) {
            database.append( { routeid: root.routeid, name: name, latitude: latitude, longitude: longitude, altitude: altitude } )
        } else {
            database.set(index, { routeid: root.routeid, name: name, latitude: latitude, longitude: longitude, altitude: altitude } )
        }
        database.refresh()
        root.update()
    }

    Component {
        id: delegate
        OptionTextItem { text: ""; button: true; }
    }
/*
    Database {
        id: database
        table: "routepoints"
        filter: root.routeid>=0? "routeid=\"" + root.routeid.toString() + "\"" : ""
        onFilterChanged: {
            console.log("RoutePointList.database.filter",database.filter)
            for (var i = 0; i<count; i++) {
                console.log("item: ",database.get(i).routeid,database.get(i).name)
            }
            root.update()
        }

        Component.onCompleted: {
            console.log("RoutePointEditPage.database.onCompleted")
            database.exec("CREATE TABLE IF NOT EXISTS routepoints (pointid INTEGER PRIMARY KEY, routeid INTEGER, name TEXT, latitude REAL, longitude REAL, altitude REAL);")
        }
    }
*/
    RoutePoints {
        id: database
        routeid: root.routeid
        onCountChanged: root.update()
        onDataChanged: root.update()
        onFilterChanged: root.update()
    }


    items: content

    DynamicItemModel {
        id: content
        name: "RouteList"
        //OptionTextItem { text: "<new>"; button: true; }
    }

    function update() {
        var item = null;
        content.clear();
        item = delegate.createObject(null)
        item.text = "<new>"
        item.button = true;
        content.append(item)
        console.log("routepointlist contains",database.count,"items")
        if (root.routeid >= 0) {
            for (var i=0; i<database.count; i++) {
                console.log("routepointlist item ",database.get(i,"name"))
                item = delegate.createObject(null)
                item.text = database.get(i).pointid + " " + database.get(i).name
                content.append(item)
            }
        }
        root.layout()
    }

    onClicked: {
        console.log("RoutepointList.onClicked",index,text);
        var point = database.get(index-1)
        routepointSelected(index-1,point.routeid,point.name,point.latitude,point.longitude,point.altitude)
    }

    Component.onCompleted: {
        update()
    }
}
