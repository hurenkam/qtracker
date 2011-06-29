import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"
import "../Map"
import "../Main"

MainOptionPage {
    id: root
    title: "Waypoint List"
    options: wptoptions
    //property MapView mapview: null
    property int catid: -1
    property int tripid: -1

    WaypointEditPage {
        id: wptedit;
        //mapview: root.mapview;
        onWaypointSaved: lst.saveWaypoint(index,name,lat,lon,alt)
    }

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
                } else {
                    wptedit.latitude = maplat;
                    wptedit.longitude = maplon;
                    wptedit.altitude = mapalt;
                }
                pageStack.push(wptedit);
            }
        }
    }
}
