import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionPage {
    id: root
    title: "Trip List"
    options: tripoptions
    property Item mapview

    TripEditPage { id: tripedit; onTripSaved: { lst.saveTrip(index,name) } onTripStopped: lst.update(); onTripStarted: lst.update() }

    ValueSpaceSubscriber  {
        id: currenttrip;
        path: "/server/trip/id"
        property int tripid: value
    }

    VisualItemModel {
        id: tripoptions

        TripList {
            id: lst
            onStartTrip: { tripedit.startTrip(currenttrip.tripid); }
            onEditTrip:  {
                tripedit.record = lst.getRecord(index)
                console.log("tripedit.record: ", tripedit.record)
                tripedit.index = index;
                pageStack.push(tripedit);
            }
            onStopTrip:  { tripedit.stopTrip(currenttrip.tripid); }
        }
    }
}

