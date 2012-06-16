import QtQuick 1.0

XmlRpc {
    id: root
    query: "/methodResponse/params/param/value/struct/member"
    signal itemFound(string name, int index)
    signal updateComplete()
    property string command: "echo"

    XmlRole { name: "name";        query: "name/string()" }
    XmlRole { name: "int";         query: "value/int/string()" }
    XmlRole { name: "double";      query: "value/double/string()" }
    XmlRole { name: "string";      query: "value/string/string()" }
    XmlRole { name: "date";        query: "value/dateTime.iso8601/string()" }

    function refresh() {
        call(command,[])
    }

    onStatusChanged: {
        //console.log("XmlRpcMap.onStatusChanged()", status)
        if (status == XmlListModel.Ready) {
            //console.log("XmlRpcMap.onStatusChanged()", root.count)
            for (var i=0; i<root.count; i++) {
                itemFound(root.get(i).name,i);
            }
            updateComplete()
        }
    }
}
