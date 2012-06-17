import QtQuick 1.0
import "../components"

XmlRpcMap {
    id: root
    //command: "time"
    path: "/time"

    property int mask: 0
    property date current
    property date elapsed
    property date monitor

    function parseDate(d) {
        var date = new Date()
        var year =   parseInt(d.substr(0,4))
        var month =  parseInt(d.substr(4,2))
        var day =    parseInt(d.substr(6,2))
        var hour =   parseInt(d.substr(9,2))
        var minute = parseInt(d.substr(12,2))
        var second = parseInt(d.substr(15,2))
        date.setFullYear(year)
        date.setMonth(month)
        date.setDate(day)
        date.setHours(hour)
        date.setMinutes(minute)
        date.setSeconds(second)
        return date
    }

    onItemFound: {
        //console.log("TimeModel.onItemFound",name,index)
        if (name == "mask")    root.mask =    root.get(index).int
        if (name == "current") root.current = root.parseDate(root.get(index).date)
        if (name == "elapsed") root.elapsed = root.parseDate(root.get(index).date)
        if (name == "monitor") root.monitor = root.parseDate(root.get(index).date)
    }

    //onUpdateComplete: console.log("TimeModel.onUpdateComplete()",mask,current,elapsed,monitor)
}

