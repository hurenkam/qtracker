import QtQuick 1.0
import "../Components"

Database {
    id: database
    table: "routes"

    Component.onCompleted: {
        //console.log("Routes.onCompleted")
        database.exec("CREATE TABLE IF NOT EXISTS routes (rte INTEGER PRIMARY KEY, name TEXT, top REAL, left REAL, bottom REAL, right REAL);")
    }
}
