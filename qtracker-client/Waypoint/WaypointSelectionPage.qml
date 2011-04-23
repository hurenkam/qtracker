import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionPage {
    id: root
    title: "Waypoint List"
    options: wptoptions

    WaypointEditPage { id: wptedit }

    VisualItemModel {
        id: wptoptions

        WaypointList {
            id: lst
            //title: "Waypoint List"
            onEditWaypoint:   { wptedit.index = index; pageStack.push(wptedit); }
        }
    }
}
