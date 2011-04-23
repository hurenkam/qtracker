import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root

    signal editWaypoint(int index)

    Component {
        id: delegate
        OptionTextItem { text: "" }
    }

    Database {
        id: database
        table: "waypoints"
        onCountChanged: lst.update()
    }

    items: content

    DynamicItemModel {
        id: content
        name: "WaypointList"
        OptionTextItem { text: "<new>"; button: true; }
    }

    function update() {
        var item = null;
        content.clear();
        console.log("waypointlist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("waypointlist item ",database.get(i,"name"))
            item = delegate.createObject(null)
            item.text = database.get(i,"waypointid") + " " + database.get(i,"name")
            content.append(item)
            lst.layout()
        }
    }

    onClicked: {
        console.log("WaypointList.onClicked",index,text);
        editWaypoint(index-1)
    }

    Component.onCompleted: update()
}
