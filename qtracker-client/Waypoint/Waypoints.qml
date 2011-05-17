import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "waypoints"
    property QtObject range: []

    function updateFilter() {
        var filter = "";
        if (range.length == 4) {
            filter +=           "longitude>=" + root.range[0].toString();
            filter += " AND " + "longitude<=" + root.range[2].toString();
            filter += " AND " + "latitude>="  + root.range[1].toString();
            filter += " AND " + "latitude<="  + root.range[3].toString();
        }
        root.filter=filter;
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
        root.exec("CREATE TABLE IF NOT EXISTS waypoints (wpt INTEGER PRIMARY KEY, name TEXT, latitude REAL, longitude REAL, altitude REAL, notes TEXT);")
        updateFilter()
    }
}
