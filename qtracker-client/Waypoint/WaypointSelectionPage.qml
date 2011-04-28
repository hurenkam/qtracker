import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"
import "../Map"

OptionPage {
    id: root
    title: "Waypoint List"
    options: wptoptions
    property MapView mapview: null

    WaypointEditPage { id: wptedit; mapview: root.mapview; onWaypointSaved: lst.saveWaypoint(index,name,lat,lon,alt) }

    VisualItemModel {
        id: wptoptions

        WaypointList {
            id: lst
            //title: "Waypoint List"
            onWaypointSelected:   {
                wptedit.index = index;
                if (index >= 0) {
                    wptedit.name = name;
                    wptedit.latitude = lat;
                    wptedit.longitude = lon;
                    wptedit.altitude = alt;
                }
                pageStack.push(wptedit);
            }
        }
    }
}
