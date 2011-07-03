import QtQuick 1.0

Item {
    id: root
    height:  50
    property bool hasRightButton: false

    onParentChanged: layoutChildren()
    onWidthChanged:  layoutChildren()
    onChildrenChanged: layoutChildren()
    function hideChildren() {
        for (var i=0; i<children.length; ++i) {
            children[i].visible = false
        }
    }

    function layoutChildren(newparent) {
        if (!newparent) return
        parent = newparent
        if (!parent.width) return
        root.width = parent.width
        for (var i=0; i<children.length; ++i) {
            if ((i==(children.length-1)) && (hasRightButton)) {
                children[i].x = root.width - root.height + 5
                children[i].y = 5
                children[i].width = root.height-10
                children[i].height = root.height-10
                children[i].visible = true
            } else {
                children[i].x = i*root.height + 5
                children[i].y = 5
                children[i].width = root.height -10
                children[i].height = root.height -10
                children[i].visible = true
            }
        }
    }
}
