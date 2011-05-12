import QtQuick 1.0
import QmlTrackerExtensions 1.0
//import "Database.js" as Engine

TableModel {
    id: root
    database: (platform==0) ? "e:\\data\\qtracker\\database.sqlite": "c:\\data\\qtracker\\database.sqlite"

    //Component.onCompleted: {
    //    Engine.setup()
    //}
}
