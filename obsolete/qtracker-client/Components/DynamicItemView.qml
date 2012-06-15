//DynamicItemView.qml
import QtQuick 1.0

Item {
    id: root
    property DynamicItemModel model

    Column {
        id: content
        anchors.fill: parent
    }

    function construct() {
        //console.log("model has",model.count(),"items")
        var l = model.count()
        var h = content.height/l
        for (var i=0; i<l; i++) {
            var item = model.get(i)
            //console.log("item",i,"content",item)
            item.x=0
            item.width=content.width
            item.y=h*i
            item.height=h
            item.parent = content
        }
    }

    Connections {
        target: root.model
        onListChanged: root.construct()
    }
    onModelChanged: construct()
    onWidthChanged: construct()
    onHeightChanged: construct()
    Component.onCompleted: construct()
}
