import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "trips"

    Component.onCompleted: {
        //console.log("Maps.onCompleted")
        root.exec("CREATE TABLE IF NOT EXISTS trips (" +
                      "trip INTEGER PRIMARY KEY, name TEXT, " +
                      "ascent REAL, descent REAL, altmin REAL, altmax REAL, altavg REAL" +
                      "speedmin REAL, speedmax REAL, speedavg REAL, " +
                      "triptime TEXT, tripdist REAL );")
    }
}
