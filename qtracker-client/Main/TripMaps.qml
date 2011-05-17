import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "tripmaps"

    Component.onCompleted: {
        //console.log("Maps.onCompleted")
        root.exec("CREATE TABLE IF NOT EXISTS tripmaps " +
                  "( trippt INTEGER PRIMARY KEY, trip INTEGER, map INTEGER );")
    }
}
