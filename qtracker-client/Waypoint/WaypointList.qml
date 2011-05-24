import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"
import "../Trip"

OptionList {
    id: root

    signal waypointSelected(int index, string name, real lat, real lon, real alt)

    TripWaypoints {
        id: tripwaypoints
    }

    ValueSpaceSubscriber  {
        id: trip;
        path: "/server/trip/id"
        property int tripid: value
    }

    function saveWaypoint(index,name,lat,lon,alt) {
        console.log("saveWaypoint",index,name,lat,lon,alt)
        if (index < 0) {
            var wpt = database.append({name: name, latitude: lat, longitude: lon, altitude: alt})
            tripwaypoints.append({wpt: wpt, trip: trip.tripid})
        } else {
            var wpt = database.set(index, {name: name, latitude: lat, longitude: lon, altitude: alt})
        }
        database.refresh()
        root.update()
        return wpt
    }

    Component {
        id: delegate
        OptionTextItem { text: ""; button: true }
    }
/*
    Database {
        id: database
        table: "waypoints"
        onCountChanged: root.update()
        onDataChanged: root.update()

        Component.onCompleted: {
            console.log("WaypointList.database.onCompleted")
            database.exec("CREATE TABLE IF NOT EXISTS waypoints (pointid INTEGER PRIMARY KEY, name TEXT, latitude REAL, longitude REAL, altitude REAL, notes TEXT);")
        }
    }
*/
    Waypoints {
        id: database
        onCountChanged: root.update()
        onDataChanged: root.update()
    }

    items: content

    DynamicItemModel {
        id: content
        name: "WaypointList"
        //OptionTextItem { text: "<new>"; button: true; }
    }

    function update() {
        var item = null;
        content.clear();
        console.log("waypointlist contains",database.count,"items")
        item = delegate.createObject(null)
        item.text = "<new>"
        item.button = true;
        content.append(item)
        for (var i=0; i<database.count; i++) {
            console.log("waypointlist item ",database.get(i).name)
            item = delegate.createObject(null)
            item.text = database.get(i).wpt + " " + database.get(i).name
            content.append(item)
        }
        root.layout()
    }

    onClicked: {
        console.log("WaypointList.onClicked",index,text);
        var wpt = database.get(index-1)
        waypointSelected(index-1,wpt.name,wpt.latitude,wpt.longitude,wpt.altitude)
    }

    Component.onCompleted: {
        update()
    }
}
