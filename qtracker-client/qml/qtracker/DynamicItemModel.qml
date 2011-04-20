import QtQuick 1.0
import "qrc:/js/dynamiclist.js" as JList

QtObject {
    id: root
    default property variant list
    property int count: 0
    property string name: "unnamed"

    function get(index) {
        return JList.getItem(index)
    }
    function append(index) {
        JList.addItem(index)
        update()
    }
    function update() {
        list = JList.getList()
        count = JList.count()
        listChanged()
    }
    function clear() {
        JList.clear()
    }

    Component.onCompleted: update()
}
