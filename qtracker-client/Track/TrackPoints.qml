import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "trackpoints"
    property int trackid: -1
    property list<QtObject> range
    range: []

    function updateFilter() {
        var filter = "";
        filter += "trk="+root.trackid.toString();
        if (range.length>0) {
            filter += " AND " + "longitude>=" + root.range[0].toString();
            filter += " AND " + "longitude<=" + root.range[2].toString();
            filter += " AND " + "latitude>="  + root.range[1].toString();
            filter += " AND " + "latitude<="  + root.range[3].toString();
        }
        root.filter=filter;
    }

    function selectRange(left, top, right, bottom) {
        root.range = [ left, top, right, bottom ];
        updateFilter();
    }

    function clearRange() {
        root.range = []
    }


    onTrackidChanged: updatefilter()
    onRangeChanged: updatefilter()
    Component.onCompleted: {
        console.log("TrackpointList.database.onCompleted")
        root.exec("CREATE TABLE IF NOT EXISTS trackpoints (trkpt INTEGER PRIMARY KEY, trk INTEGER, time TEXT, latitude REAL, longitude REAL, altitude REAL, time TEXT);")
        updatefilter()
    }
}
