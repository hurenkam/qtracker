import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "waypoints"
    property variant range: []

    function updateFilter() {
        if (!range.length) return;

        var filter = "";
        if (range.length == 4) {
            filter +=           "longitude>=" + root.range[0].toString();
            filter += " AND " + "longitude<=" + root.range[2].toString();
            filter += " AND " + "latitude>="  + root.range[1].toString();
            filter += " AND " + "latitude<="  + root.range[3].toString();
        }
        root.filter=filter;
        console.log("Waypoints.updateFilter(): ", filter)
        for (var i=0; i<count; i++) {
            console.log("Point: ",get(i).name,get(i).latitude,get(i).longitude)
        }
    }

    function selectRange(left, top, right, bottom) {
        root.range = [ left, top, right, bottom ];
    }

    function clearRange() {
        root.range = []
    }

    onRangeChanged: updateFilter()
    Component.onCompleted: {
        console.log("Waypoints.onCompleted")
        root.exec("CREATE TABLE IF NOT EXISTS waypoints (wpt INTEGER PRIMARY KEY, name TEXT, time TEXT, latitude REAL, longitude REAL, altitude REAL, notes TEXT);")
        updateFilter()
    }
}
