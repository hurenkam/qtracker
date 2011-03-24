import QtQuick 1.0

Database {
    id: root
    table: "settings"

    function setValue(key,value) {
    }

    function getValue(key,value) {
    }

    Component.onCompleted: createTable("key text unique, value text")
}
