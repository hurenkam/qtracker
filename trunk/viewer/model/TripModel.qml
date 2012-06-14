import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    command: "trip"

    property int key: 0
    property string name:  "<none>"
    property string state: "Idle"

    onItemFound: {
        //console.log("TripModel.onItemFound",name,index,root.get(index))
        if (name == "id")    root.key =   root.get(index).int
        if (name == "name")  root.name =  root.get(index).string
        if (name == "state") root.state = root.get(index).string
    }

    onUpdateComplete: console.log("TripModel.onUpdateComplete()",key,name,state)
}

