import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    path: "/track"

    signal dataChanged(int key, string name, string state);

    property int key: 0
    property string name:  "<none>"
    property string state: "Idle"

    onItemFound: {
        //console.log("TrackModel.onItemFound",name,index,root.get(index))
        if (name == "id")    root.key =   root.get(index).int
        if (name == "name")  root.name =  root.get(index).string
        if (name == "state") root.state = root.get(index).string
    }

    onUpdateComplete: {
        dataChanged(key,name,state)
        //console.log("TrackModel.onUpdateComplete()",key,name,state)
    }
}

