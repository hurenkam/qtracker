import QtQuick 1.0
import "../Components"

Database {
    id: database
    table: "mappoints"

    property int mapid: -1
    filter: mapid>=0? "mapid='" + mapid.toString() + "'" : ""
    onMapidChanged: {
        console.log("Refpoints.onMapidChanged(",mapid,")")
        if (mapid>=0) {
            filter = "mapid='" + mapid.toString() + "'"
        } else {
            filter = ""
        }
    }

    Component.onCompleted: {
        console.log("Refpoints.onCompleted")
        database.exec("CREATE TABLE IF NOT EXISTS mappoints (mappt INTEGER PRIMARY KEY, " +
                      "mapid INTEGER, name TEXT, " +
                      "latitude NUMBER, longitude NUMBER, x NUMBER, y NUMBER" +
                      ");")
    }
}
