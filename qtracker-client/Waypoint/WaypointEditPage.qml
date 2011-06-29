import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../Components"

OptionPage {
    id: root
    title: (wptid==-1) ? "New Waypoint" : "Edit Waypoint"
    options: wptoptions
    confirmbutton: true
    property int wptid: -1
    property TWaypoint dbrecord

    signal waypointSaved(int wptid)

    Settings {
        id: settings
    }

    VisualItemModel {
        id: wptoptions

        OptionList {
            id: wptbox
            items: wptitems

            DynamicItemModel {
                id: wptitems
                name: "wptitems"

                OptionInputItem { id: wptname;        text: "Name:     "; onValueChanged: settings.setProperty("wpt_defaultname",value) }
                OptionInputItem { id: wptlat;         text: "Latitude: "; value: "0.0" }
                OptionInputItem { id: wptlon;         text: "Longitude:"; value: "0.0" }
                OptionInputItem { id: wptalt;         text: "Longitude:"; value: "0.0" }

                Component.onCompleted: {
                    refname.value = settings.getProperty("wpt_defaultname","wpt-000")
                }
            }
        }
    }

    function refreshData() {
        if (wptid==-1) {
            wptname.value = settings.getProperty("wpt_defaultname","wpt-000")
            wptlat.value  = 0.0
            wptlon.value  = 0.0
            wptalt.value  = 0.0
        } else {
            dbrecord = database.getWaypoint(wptid)
            wptname.value = dbrecord.name
            wptlat.value = dbrecord.latitude
            wptlon.value = dbrecord.longitude
            wptalt.value = dbrecord.altitude
        }
    }

    function saveWaypoint() {
        if (wptid>0) {
            dbrecord.name = wptname.value
            dbrecord.latitde = wptlat.value
            dbrecord.longitude = wptlon.value
            dbrecord.altitude = wptalt.value
            //dbrecord.save()
            //root.waypointSaved(root.wptid);
        }
    }

    onWptidChanged: refreshData()
    Component.onCompleted: refreshData()
    onConfirm: {
        console.log("WaypointEditPage.onConfirm",wptname.value,wptlat.value,wptlon.value,wptx.value,wpty.value)
        saveWaypoint()
        pageStack.pop()
    }
}
