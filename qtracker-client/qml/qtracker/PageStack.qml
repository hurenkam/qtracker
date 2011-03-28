import QtQuick 1.0
import "qrc:/js/pages.js" as JS

Rectangle {
    id: root
    color: "black"
    x: -pwidth
    y: 0
    width: 0
    height: parent.height
    property real pwidth: parent.width
    property real pheight: parent.height
    onPwidthChanged: resizeChildren()
    onPheightChanged: resizeChildren()
    Behavior on x { NumberAnimation { easing.type: Easing.InOutQuart; duration: 400 } }

    Timer {
        id: disposeTimer;
        interval: 500;
        repeat: false;
        onTriggered: delayedDispose()

        property Item oldpage: null

        function delayedDispose() {
            console.log("delayedDispose")
            if (!oldpage) return;
            if (oldpage.page.parent == oldpage) {
                oldpage.page.parent = null;
                oldpage.page.visible = false;
            }
            oldpage.page = null;
            oldpage.destroy();
        }

        function dispose(page) {
            oldpage = page;
            start();
        }
    }

    function resizeChildren() {
        for (var i=0;i<JS.length();i++) {
            JS.stack[i].x = i * parent.width;
            JS.stack[i].width = parent.width;
            JS.stack[i].page.width = parent.width;
            JS.stack[i].height = parent.height;
            JS.stack[i].page.height = parent.height;
        }
        x = -(JS.length()-1)*parent.width
        width =  parent.width * JS.length()
        height = pheight
    }

    function push(page) {
        var newpage = Qt.createQmlObject("import QtQuick 1.0; Rectangle { y:0; property Item page: null }",root);
        newpage.color = "black"
        newpage.page = page
        JS.push(newpage);

        page.parent = newpage;
        page.pageStack = root;
        page.visible = true

        resizeChildren();
    }

    function pop() {
        if (JS.length()<1) return;
        var oldpage = JS.pop();
        disposeTimer.dispose(oldpage);
        resizeChildren();
    }
}
