import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    path: "/speed"

    property int mask: 0
    property double current: 0.0
    property double average: 0.0
    property double minimum: 0.0
    property double maximum: 0.0

    onItemFound: {
        //console.log("SpeedModel.onItemFound",name,index,root.get(index))
        if (name == "mask")    root.mask =    root.get(index).int
        if (name == "current") root.current = root.get(index).double
        if (name == "average") root.average = root.get(index).double
        if (name == "minimum") root.minimum = root.get(index).double
        if (name == "maximum") root.maximum = root.get(index).double
    }

    onUpdateComplete: console.log("SpeedModel.onUpdateComplete()",mask,current,average,minimum,maximum)
}

