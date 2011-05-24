import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"
import "../Trip"

OptionList {
    id: root

    signal recordTrack(int index)
    signal editTrack(int index)
    signal stopTrack(int index)

    Component {
        id: delegate
        OptionTextItem { text: "" }
    }

    Tracks {
        id: database
        onCountChanged: root.update()
        onDataChanged: root.update()
    }

    TripTracks {
        id: triptracks
    }

    items: content

    DynamicItemModel {
        id: content
        name: "TrackList"
        //OptionTextItem { text: "<new>"; button: true; }
    }

    ValueSpaceSubscriber  {
        id: trackstatus;
        path: "/server/track/status"
        property string status: value
        onStatusChanged: { console.log("TrackMenu.trackstatus:onStatusChanged: ", status); root.update() }
        Component.onCompleted: console.log("TrackMenu.trackstatus:onCompleted: ", value)
        //OptionTextItem { text: trackstatus.status=="idle"? "Start Track": "Stop Track" }
    }

    ValueSpaceSubscriber  {
        id: trackname;
        path: "/server/track/name"
        property string name: value
    }

    ValueSpaceSubscriber  {
        id: trip;
        path: "/server/trip/id"
        property int tripid: value
    }

    function saveTrack(index,name,interval) {
        console.log("TrackList.saveTrack",index,name,interval)
        var trk
        if (index < 0) {
            trk = database.append({name: name, interval: interval})
            triptracks.append({trk: trk, trip: trip.tripid})
        } else {
            database.set(index, {name: name, interval: interval})
            trk = database.get(index).trk
        }
        database.refresh()
        root.update()
        return trk
    }

    function update() {
        var item = null;
        console.log("TrackList.update(): content.count() after content.clear()", content.count())
        item = delegate.createObject(null)
        item.text = trackstatus.status=="idle"? "<new>" : "<stop>"
        item.button = true;
        content.clear();
        content.append(item)
        console.log("tracklist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("tracklist item ",database.get(i).name)
            item = delegate.createObject(null)
            item.text = database.get(i).trk + " " + database.get(i).name
            content.append(item)
        }
        root.layout()
    }

    onClicked: {
        console.log("TrackList.onClicked",index,text);
        if (index == 0) {
            if (trackstatus.status == "idle") {
                recordTrack(index-1)
            } else {
                stopTrack(index-1)
            }
        } else {
            editTrack(index-1)
        }
    }

    Component.onCompleted: update()
}
