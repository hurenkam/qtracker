import QtQuick 1.0

Database {
    id: database
    table: "maps"

    Component.onCompleted: {
        //console.log("Maps.onCompleted")
        database.exec("CREATE TABLE IF NOT EXISTS maps (map INTEGER PRIMARY KEY, " +
                      "name TEXT, file TEXT, angle REAL" +
                      "bx REAL, by REAL, blat REAL, blon REAL" +
                      "dx REAL, dy REAL, dlat REAL, dlon REAL" +
                      "top REAL, left REAL, bottom REAL, right REAL);")
    }
}
