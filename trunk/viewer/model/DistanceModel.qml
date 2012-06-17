import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    path: "/distance"
    signal dataChanged(double current, double monitor)

    property int mask: 0
    property double current: 0.0
    property double monitor: 0.0

    onItemFound: {
        //console.log("DistanceModel.onItemFound",name,index,root.get(index))
        if (name == "mask")    root.mask =    root.get(index).int
        if (name == "current") root.current = root.get(index).double
        if (name == "monitor") root.monitor = root.get(index).double
    }

    onUpdateComplete: {
        dataChanged(current,monitor)
        console.log("DistanceModel.onUpdateComplete()",mask,current,monitor)
    }
}

