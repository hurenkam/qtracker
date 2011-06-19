import QtQuick 1.0
import "../Components"

Database {
    id: database
    table: "mappoints"

    Component.onCompleted: {
        //console.log("Maps.onCompleted")
        database.exec("CREATE TABLE IF NOT EXISTS mappoints (mappt INTEGER PRIMARY KEY, " +
                      "mapid INTEGER, name TEXT, " +
                      "latitude NUMBER, longitude NUMBER, x NUMBER, y NUMBER" +
                      ");")
    }
}
