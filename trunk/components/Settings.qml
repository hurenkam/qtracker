import QtQuick 1.0
import QmlTrackerExtensions 1.0

Item {
    id: root

    TDatabase {
        id: db
    }

    function setProperty(key,newvalue) {
        db.set(key,newvalue)
    }

    function getProperty(key,defaultvalue) {
        return db.get(key,defaultvalue)
    }
}
