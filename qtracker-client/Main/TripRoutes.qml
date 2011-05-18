import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "triproutes"

    Component.onCompleted: {
        //console.log("Maps.onCompleted")
        root.exec("CREATE TABLE IF NOT EXISTS triproutes " +
                  "( triprte INTEGER PRIMARY KEY, trip INTEGER, rte INTEGER );")
    }
}
