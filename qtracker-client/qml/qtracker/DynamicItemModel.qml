import QtQuick 1.0
import "qrc:/js/dynamiclist.js" as JList

Item {
    id: root
    property variant list: JList.getList()
    property string name

    signal listAppended()
    signal childrenMoved(int count)
    signal listCleared()

    function get(index) {
        return JList.getItem(index)
    }
    function append(index) {
        JList.addItem(index)
        listAppended()
    }
    function count() {
        return JList.count()
    }
    function movechildren() {
        var length = children.length
        if (length<1) return;
        for (var i=0; i<length; i++) {
            console.log(children,children[i])
            append(children[i])
        }
        childrenMoved(length)
    }
    function clear() {
        JList.clear()
        listCleared()
    }

    Component.onCompleted: {
        //console.log("DynamicItemModel has",children.length,"child(ren)")
        movechildren()
    }
}
