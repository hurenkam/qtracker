import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    command: "location"

    property int mask: 0
    property double latitude:  0.0
    property double longitude: 0.0
    property double altitude:  0.0

    onItemFound: {
        //console.log("LocationModel.onItemFound",name,index,root.get(index))
        if (name == "mask")      root.mask =      root.get(index).int
        if (name == "latitude")  root.latitude =  root.get(index).double
        if (name == "longitude") root.longitude = root.get(index).double
        if (name == "altitude")  root.altitude =  root.get(index).double
    }

    onUpdateComplete: console.log("AltitudeModel.onUpdateComplete()",mask,latitude,longitude,altitude)
}

