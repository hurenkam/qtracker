import QtQuick 1.0
import QmlTrackerExtensions 1.0
//import "Database.js" as Engine

TableModel {
    id: root
    //database: (client.platform==0) ? "e:\\data\\qtracker\\database.sqlite": "c:\\data\\qtracker\\database.sqlite"
    //database: "e:\\data\\qtracker\\database.sqlite"
    database: "c:\\data\\qtracker\\database.sqlite"

    //Component.onCompleted: {
    //    Engine.setup()
    //}
}

/*
Item {
    id: root
    property string database: "c:\\data\\qtracker\\database.sqlite"
    property string table: "tables"
    property int count: 0
}
*/
