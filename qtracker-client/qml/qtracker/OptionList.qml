import QtQuick 1.0

Item {
    id: root
    property alias title: header.text
    property QtObject items: null
    property alias boxheight: flickable.height
    //height: flickable.height + header.offset + 24

    signal clicked(int index, string text)

    Text {
        id: header
        x: 20
        y: 20
        color: "#505050"
        style: Text.Sunken
        styleColor: "black"
        text: ""
        property int offset: (text=="")? 10 : 50
    }

    Flickable {
        id: flickable
        x: 0
        y: header.offset
        width: root.width
        //height: 360 - header.offset
        //height: 100
        height: root.height -header.offset
        contentWidth: root.width
        contentHeight: box.height+20
        interactive: contentHeight > height? true: false
        clip: true

        Rectangle {
            id: box

            x: 10
            y: 10
            width: root.width -20
            height: content.height
            radius: 12
            color: "white"
            border.color: "grey"
            border.width: 1

            Item {
                id: content
                width: box.width
                //anchors.left: box.left
                //anchors.right: box.right
                y: 0
            }

            Item {
                id: seperators
                //anchors.left: box.left
                //anchors.right: box.right
                width: box.width
                y: 0

                function clear() {
                    for (var i=0; i<seperators.children.length; ++i) {
                        seperators.children[i].destroy();
                    }
                }
            }


            function layout() {
                if (items) {
                    items.parent = content;
                    items.x = 0;
                    items.y = 0;
                    items.width = box.width;
                    layoutItems(box.width);
                    items.height = content.height;
                }
            }

            function layoutItems(w) {
                //console.log("got", items.children.length, "items")
                var h = 2;
                seperators.clear();
                var seperator;
                for (var i=0; i<items.children.length; ++i) {
                    h = layoutOptionItem(items.children[i],w,h+10,i) + 10;
                    if ((i+1) < items.children.length) {
                        seperator = line.createObject(seperators)
                        h = h+1
                        seperator.y = h
                    }
                }
                content.height = h+2;
            }

            function layoutOptionItem(item,w,h,i) {
                item.x = 10;
                item.y = h;
                item.width = w-20;
                h = h + item.height;
                //console.log("item:",item.x,item.y,item.width,item.height);
                return h;
            }

            function initialise() {
                for (var i=0; i<items.children.length; ++i) {
                    items.children[i].clicked.connect(root.clicked)
                    items.children[i].button = true;
                    items.children[i].index = i;
                }
            }

            Component.onCompleted: { initialise(); layout(); }
            onWidthChanged:        layout();
        }
    }

    Component {
        id: line

        Rectangle {
            x: 0
            //y: box.height/items.length * (index+1)
            width: box.width
            height: 1
            color: "grey"

            //Component.onCompleted: console.log("line:",index + 1)
        }
    }

    function layout() {
        box.layout()
    }

    onItemsChanged:         box.layout();
    onHeightChanged:        box.layout();
}
