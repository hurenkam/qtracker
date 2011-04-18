import QtQuick 1.0

QtObject {
    id: root
    default property alias children: container.children
    property int count: container.children.length
    property alias parent: container.parent

    property Item c: Item {
        id: container
        parent: null
        anchors.fill: parent

        function append(item) {
            item.parent = container
            //childrenChanged()
        }
        function clear() {
            children=[]
            //childrenChanged()
        }

        onChildrenChanged: console.log("DynamicItemModel.onChildrenChanged",container.children.length)
    }

    function append(item) {
        console.log("DynamicItemModel.append()",item)
        container.append(item)
    }
    function clear() {
        console.log("DynamicItemModel.clear()")
        container.clear()
    }

    Component.onCompleted: console.log("DynamicItemModel.onCompleted")
}
