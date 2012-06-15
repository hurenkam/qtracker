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
    leftbuttonsrc: "../Images/left-plain.png"
    rightbuttonsrc: "../Images/visible-plain.png"

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
                    wptname.value = settings.getProperty("wpt_defaultname","wpt-000")
                }
            }
        }
    }

    function refreshData() {
        dbrecord = database.getWaypoint(wptid)
        if (wptid==-1) {
            wptname.value = settings.getProperty("wpt_defaultname","wpt-000")
            wptlat.value  = 0.0
            wptlon.value  = 0.0
            wptalt.value  = 0.0
        } else {
            wptname.value = dbrecord.name
            wptlat.value = dbrecord.latitude
            wptlon.value = dbrecord.longitude
            wptalt.value = dbrecord.altitude
        }
    }

    function saveWaypoint() {
        dbrecord.name = wptname.value
        dbrecord.latitude = wptlat.value
        dbrecord.longitude = wptlon.value
        dbrecord.altitude = wptalt.value
        dbrecord.save()
        root.waypointSaved(dbrecord.wptid);
        root.wptid = dbrecord.wptid
    }

    onWptidChanged: refreshData()
    Component.onCompleted: refreshData()
    onConfirm: {
        console.log("WaypointEditPage.onConfirm",wptname.value,wptlat.value,wptlon.value,wptalt.value)
        saveWaypoint()
        pageStack.pop()
    }
}
