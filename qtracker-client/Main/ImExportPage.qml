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
                tripedit.index = index;
                tripedit.name = name;
                pageStack.push(tripedit);
            }
            onStopTrip:  { tripedit.stopTrip(currenttrip.tripid); }
        }
    }
}

/*
OptionPage {
    id: root
    title: "Settings"
    options: settingsmodel
    property Item mapview

    OptionPage { id: security; title: "Security"}

    VisualItemModel {
        id: settingsmodel

        OptionList {
            id: box1
            title: "Security"
            items: securityitems

            DynamicItemModel {
                id: securityitems
                name: "securityitems"
                OptionTextItem { text: "Passcode"; button: true; onClicked: root.pageStack.push(security) }
            }
        }

        OptionList {
            id: box2
            title: "General"
            items: generalitems

            DynamicItemModel {
                id: generalitems
                name: "generalitems"

                OptionTextItem { text: "Days End" }
                OptionTextItem { text: "Moment Sorting" }
                OptionTextItem { text: "Week Starts On" }
                OptionTextItem { text: "Sounds" }
                OptionTextItem { text: "Photos" }
            }
        }

        OptionList {
            id: box3
            title: "Other"
            height: 170
            items: otheritems

            DynamicItemModel {
                id: otheritems
                property string name: "otheritems"
                OptionTextItem { text: "Option a"; button: true }
                OptionTextItem { text: "Option b"; button: true }
                OptionTextItem { text: "Option c"; button: true }
                OptionTextItem { text: "Option d"; button: true }
                OptionTextItem { text: "Option e"; button: true }
                OptionTextItem { text: "Option f"; button: true }
                OptionTextItem { text: "Option g"; button: true }
                OptionTextItem { text: "Option h"; button: true }
            }

            onClicked: console.log("other.onclicked:",index,text)
        }

    }
}
*/
