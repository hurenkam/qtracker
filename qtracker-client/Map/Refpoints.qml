import QtQuick 1.0

Database {
    id: database
    table: "refpoints"

    Component.onCompleted: {
        //console.log("Maps.onCompleted")
        database.exec("CREATE TABLE IF NOT EXISTS refpoints (refpt INTEGER PRIMARY KEY, " +
                      "name TEXT, map INTEGER, " +
                      "latitude REAL, longitude REAL, x REAL, y REAL" +
                      ");")
    }
}
