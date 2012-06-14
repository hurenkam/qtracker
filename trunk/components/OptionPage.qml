import QtQuick 1.0

Page {
    id: root
    imageSource: "options-bg.png"
    property alias backbutton: hdr.leftButtonVisible
    property alias confirmbutton: hdr.rightButtonVisible
    property alias rightbutton: hdr.rightButtonVisible
    property alias rightbuttonsrc: hdr.rightButtonSource
    property alias rightbuttonradius: hdr.rightButtonRadius
    property alias leftbutton: hdr.leftButtonVisible
    property alias leftbuttonsrc: hdr.leftButtonSource
    property alias leftbuttonradius: hdr.rightButtonRadius
    property alias title: hdr.text

    signal confirm()
    signal rightClicked()

    function cancel() {
        pageStack.pop();
    }

    property Item tools: null

    Item {
        id: title
        x: 0;
        y: 0;
        width: root.width;
        height: visible? parent.height/16 : 0;
        z: 5;
    }
    ToolBar {
        id: toolbar
        tools:root.tools
        visible: tools? true: false
        x: 0;
        y: visible? parent.height-tools.height : parent.height;
        width: root.width;
        height: visible? tools.height : 0;
        z: 5
    }
    Flickable {
        //id: content
        anchors.top: title.bottom
        anchors.bottom: toolbar.top
        width: root.width
        contentWidth: content.width
        contentHeight: content.height
        interactive: contentHeight > height? true: false
        clip: true
        z: 10

        Item {
            id: content
        }
    }

    property Item header: OptionHeader {
        id: hdr
        //visible: root.tools? false: true
        text: "Options"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
        rightButtonVisible: false
        rightButtonSource: "visible.png"
        onRightClicked: { root.confirm(); root.rightClicked(); }
        buttonheight: title.height
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
