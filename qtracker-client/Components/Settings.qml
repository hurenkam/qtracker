import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../Components"

Database {
    id: root
    table: "settings"

    function setProperty(key,newvalue) {
        console.log("setProperty",key,newvalue)
        root.filter = "setting=\""+key+"\""
        console.log("filter",root.filter,"count",root.count)
        if (root.count < 1) {
            append({ setting: key, value: newvalue } );
        }
        else {
            set(0,{ setting: key, value: newvalue } );
        }
    }

    function getProperty(key,defaultvalue) {
        console.log("getProperty",key,defaultvalue)
        root.filter = "setting=\""+key+"\""
        if (root.count < 1) {
            append({ setting: key, value: defaultvalue } );
            console.log("result",defaultvalue)
            return defaultvalue
        }
        else {
            console.log("result",root.get(0).value)
            return root.get(0).value
        }
    }

    Component.onCompleted: {
        console.log("Settings.onCompleted")
        root.exec("CREATE TABLE IF NOT EXISTS settings (setting TEXT UNIQUE, value TEXT)");
    }
}
