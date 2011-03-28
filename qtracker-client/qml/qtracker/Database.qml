import QtQuick 1.0
import QmlTrackerExtensions 1.0

TableModel {
    id: root
    database: (client.platform==0) ? "e:\\data\\qtracker\\database.sqlite": "c:\\data\\qtracker\\database.sqlite"
}
