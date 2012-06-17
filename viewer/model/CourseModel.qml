import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    path: "/course"

    property int mask: 0
    property double current: 0.0
    property double average: 0.0
    property double monitor: 0.0

    onItemFound: {
        //console.log("CourseModel.onItemFound",name,index,root.get(index))
        if (name == "mask")    root.mask =    root.get(index).int
        if (name == "current") root.current = root.get(index).double
        if (name == "average") root.average = root.get(index).double
        if (name == "monitor") root.monitor = root.get(index).double
    }

    onUpdateComplete: console.log("CourseModel.onUpdateComplete()",mask,current,average,monitor)
}
