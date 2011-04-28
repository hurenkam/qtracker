import QtQuick 1.0

Page {
    id: root
    imageSource: "options-bg.png"
    property alias backbutton: hdr.leftButtonVisible
    property alias confirmbutton: hdr.rightButtonVisible
    property alias title: hdr.text

    signal confirm()

    function cancel() {
        pageStack.pop();
    }

    Item {
        id: title
        x: 0;
        y: 0;
        width: root.width;
        height: 50;
    }
    Flickable {
        //id: content
        anchors.top: title.bottom
        anchors.bottom: root.bottom
        width: root.width
        contentWidth: content.width
        contentHeight: content.height
        interactive: contentHeight > height? true: false
        clip: true

        Item {
            id: content
        }
    }

    property Item header: OptionHeader {
        id: hdr
        text: "Options"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
        rightButtonVisible: false
        rightButtonSource: "visible.svg"
        onRightClicked: root.confirm();
    }

    property QtObject options: null;

    function layoutPage() {
        header.parent = title;
        header.anchors.fill = title;

        if (options) {
            //options.parent = content;
            layoutOptions(root.width);
        }
    }

    function layoutOptions(w) {
        //console.log("got", options.children.length, "options")
        content.width = w;
        var h = 0;
        for (var i=0; i<options.children.length; ++i) {
            h = layoutOptionBox(options.children[i],w,h);
        }
        content.height = h+20;
    }

    function layoutOptionBox(child,w,h) {
        if (child.visible) {
            child.parent = content;
            child.x = 0;
            child.y = h;
            child.width = w;
            h = h + child.height;
            //console.log("child:",child.x,child.y,child.width,child.height);
        }
        return h;
    }

    onWidthChanged: layoutPage();
    onHeightChanged: layoutPage();
    onOptionsChanged: layoutPage();
    Component.onCompleted: layoutPage();
}
