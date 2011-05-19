import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "triptracks"

    Component.onCompleted: {
        //console.log("Maps.onCompleted")
        root.exec("CREATE TABLE IF NOT EXISTS triptracks " +
                  "( triptrk INTEGER PRIMARY KEY, trip INTEGER, trk INTEGER );")
    }
}
