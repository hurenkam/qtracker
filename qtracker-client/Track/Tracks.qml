import QtQuick 1.0
import "../Components"

Database {
    id: root
    table: "tracks"

    Component.onCompleted: {
        //console.log("TrackList.database.onCompleted")
        database.exec("CREATE TABLE IF NOT EXISTS tracks (trackid INTEGER PRIMARY KEY, name TEXT, interval INTEGER, top REAL, left REAL, bottom REAL, right REAL);")
    }
}
