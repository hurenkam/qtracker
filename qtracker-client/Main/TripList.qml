import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root

    signal startTrip(int index)
    signal stopTrip(int index)
    signal editTrip(int index, string name)

    Component {
        id: delegate
        OptionTextItem { text: ""; button: true; }
    }

    Trips {
        id: database
        onCountChanged: root.update()
        onDataChanged: root.update()
    }

    items: content

    DynamicItemModel {
        id: content
        name: "TripList"
    }

    ValueSpaceSubscriber  {
        id: tripstatus;
        path: "/server/trip/status"
        property string status: value
        onStatusChanged: { console.log("TripMenu.tripstatus:onStatusChanged: ", status); root.update() }
        Component.onCompleted: console.log("TripMenu.tripstatus:onCompleted: ", value)
    }

    ValueSpaceSubscriber  {
        id: tripname;
        path: "/server/trip/name"
        property string name: value
    }

    ValueSpaceSubscriber  {
        id: tripid;
        path: "/server/trip/id"
        property string name: value
    }

    function saveTrip(index,name) {
        console.log("TripList.saveTrip",index,name)
        var trip
        if (index < 0) {
            trip = database.append({name: name})
        } else {
            database.set(index, {name: name})
            trip = database.get(index).trip
        }
        database.refresh()
        root.update()
        return trip
    }

    function update() {
        var item = null;
        console.log("TrackList.update(): content.count() after content.clear()", content.count())
        item = delegate.createObject(null)
        item.text = tripstatus.status=="started"? "<stop>" : "<start>"
        item.button = true;
        content.clear();
        content.append(item)
        console.log("triplist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("triplist item ",database.get(i).name)
            item = delegate.createObject(null)
            item.text = database.get(i).trip + " " + database.get(i).name
            content.append(item)
        }
        root.layout()
    }

    onClicked: {
        console.log("TripList.onClicked",index,text);
        if (index == 0) {
            if (tripstatus.status == "stopped") {
                startTrip(index-1)
            } else {
                stopTrip(index-1)
            }
        } else {
            editTrip(index-1,database.get(index-1).name)
        }
    }

    Component.onCompleted: update()
}
