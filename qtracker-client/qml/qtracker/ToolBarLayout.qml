import QtQuick 1.0

Item {
    id: root
    height:  50
    property bool hasRightButton: false

    onParentChanged: layoutChildren()
    onWidthChanged:  layoutChildren()
    onChildrenChanged: layoutChildren()
    function layoutChildren(newparent) {
        if (!newparent) return
        parent = newparent
        if (!parent.width) return
        root.width = parent.width
        for (var i=0; i<children.length; ++i) {
            //if (i==0) {
            //    children[i].x = 5
            //    children[i].y = 5
            //    children[i].width = 40
            //    children[i].height = 40
            //} else
            if ((i==(children.length-1)) && (hasRightButton)) {
                children[i].x = root.width - 45
                children[i].y = 5
                children[i].width = 40
                children[i].height = 40
            } else {
                children[i].x = i*50 + 5
                children[i].y = 5
                children[i].width = 40
                children[i].height = 40
            }
        }
    }
}
