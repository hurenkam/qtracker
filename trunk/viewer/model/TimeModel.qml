import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    command: "time"

    property int mask: 0
    property string current: ""
    property string elapsed: ""
    property string monitor: ""

    onItemFound: {
        //console.log("TimeModel.onItemFound",name,index)
        if (name == "mask")    root.mask =    root.get(index).int
        if (name == "current") root.current = root.get(index).date
        if (name == "elapsed") root.elapsed = root.get(index).date
        if (name == "monitor") root.monitor = root.get(index).date
    }

    onUpdateComplete: console.log("TimeModel.onUpdateComplete()",mask,current,elapsed,monitor)
}

