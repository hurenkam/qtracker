import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "tripwaypoints"

    Component.onCompleted: {
        //console.log("Maps.onCompleted")
        root.exec("CREATE TABLE IF NOT EXISTS tripwaypoints " +
                  "( tripwpt INTEGER PRIMARY KEY, trip INTEGER, wpt INTEGER );")
    }
}
