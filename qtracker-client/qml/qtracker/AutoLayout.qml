import QtQuick 1.0

Item {
    id: root
    property bool hasRightButton: false
    property bool sideBySide: width>height

    onSideBySideChanged:   layoutChildren()
    onChildrenChanged:     layoutChildren()
    Component.onCompleted: layoutChildren()
/*
    function connectChildren() {
        for (i=0; i<c; i++)
            Connections { target: children[i]; onVisibilityChanged: root.layoutChildren; }
    }
*/
    function layoutChildren() {
        if (!parent) return
        if (!parent.width) return
        if (!parent.height) return
        if (!children.length) return
        var c = children.length
        var v = 0;
        for (i=0; i<c; i++)
            if (children[i].visible) v++
        if (!v) return

        console.log("AutoLayout.layoutChildren",c,v,sideBySide)

        if (sideBySide) {
            var w = parent.width / v
            var j = 0;
            for (var i=0; i<c; ++i) {
                if (children[i].visible) {
                    children[i].x = j*w
                    children[i].y = 0
                    children[i].width = w
                    children[i].height = parent.height
                    j=j+1
                }
                console.log("layout: ",i,children[i].visible,children[i].x,children[i].y,children[i].width,children[i].height)
            }
        } else {
            var h = parent.height / v
            var j = 0;
            for (var i=0; i<c; ++i) {
                if (children[i].visible) {
                    children[i].x = 0
                    children[i].y = j*h
                    children[i].width = parent.width
                    children[i].height = h
                    j=j+1
                }
                console.log("layout: ",i,children[i].visible,children[i].x,children[i].y,children[i].width,children[i].height)
            }
        }
    }
}
