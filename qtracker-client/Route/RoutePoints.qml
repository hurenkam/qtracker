import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "routepoints"
    property int routeid: -1
    filter: routeid>=0? "routeid=\"" + routeid.toString() + "\"" : ""
/*
    onFilterChanged: {
        console.log("RoutePoints.filter",filter)
        for (var i = 0; i<count; i++) {
            console.log("item: ",get(i).routeid,get(i).name)
        }
    }
*/
    Component.onCompleted: {
        console.log("RoutePointEditPage.database.onCompleted")
        database.exec("CREATE TABLE IF NOT EXISTS routepoints (rtept INTEGER PRIMARY KEY, rte INTEGER, name TEXT, latitude REAL, longitude REAL, altitude REAL);")
    }
}
