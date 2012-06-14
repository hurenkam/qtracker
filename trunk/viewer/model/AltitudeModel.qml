import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    command: "altitude"

    property int mask: 0
    property double current: 0.0
    property double average: 0.0
    property double minimum: 0.0
    property double maximum: 0.0
    property double ascent:  0.0
    property double descent: 0.0
    property double monitor: 0.0

    onItemFound: {
        //console.log("AltitudeModel.onItemFound",name,index,root.get(index))
        if (name == "mask")    root.mask =    root.get(index).int
        if (name == "current") root.current = root.get(index).double
        if (name == "average") root.average = root.get(index).double
        if (name == "minimum") root.minimum = root.get(index).double
        if (name == "maximum") root.maximum = root.get(index).double
        if (name == "ascent")  root.ascent  = root.get(index).double
        if (name == "descent") root.descent = root.get(index).double
        if (name == "monitor") root.monitor = root.get(index).double
    }

    onUpdateComplete: console.log("AltitudeModel.onUpdateComplete()",mask,current,average,minimum,maximum,ascent,descent)
}

