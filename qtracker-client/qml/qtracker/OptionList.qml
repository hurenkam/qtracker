import QtQuick 1.0

Item {
    id: root
    property alias title: header.text
    property QtObject items: null
    height: header.offset + box.height + 10

    signal clicked(int index, string text)

    Text {
        id: header
        x: 20
        y: 20
        color: "#505050"
        style: Text.Sunken
        styleColor: "black"
        text: ""
        property int offset: (text=="")? 10 : 40
    }

    Flickable {
        id: flickable
        x: 0
        y: header.offset + 10
        width: root.width
        //height: 360 - header.offset
        //height: 100
        height: root.height -header.offset
        contentWidth: root.width
        contentHeight: box.height+2
        interactive: contentHeight > height? true: false
        clip: true

        Rectangle {
            id: box

            x: 10
            y: 0
            width: root.width -20
            height: content.height
            radius: 12
            color: "white"
            border.color: "grey"
            border.width: 1

            Item {
                id: content
            }

            Item {
                id: seperators

                function clear() {
                    for (var i=0; i<seperators.children.length; ++i) {
                        seperators.children[i].destroy();
                    }
                }
            }

            function layout() {
                if (items) {
                    //items.parent = content
                    content.height = layoutItems(box.width);
                }
                else
                {
                    content.height = 30
                }
            }

            function layoutItems(w) {
                //console.log("got", items.children.length, "items")
                var h = 2;
                seperators.clear();
                var seperator;
                for (var i=0; i<items.count; ++i) {
                    h = layoutOptionItem(items.get(i),w,h+10) + 10;
                    if ((i+1) < items.count) {
                        seperator = line.createObject(seperators)
                        h = h+1
                        seperator.x = 0
                        seperator.width = w
                        seperator.y = h
                    }
                }
                return h+2;
            }

            function layoutOptionItem(item,w,h) {
                item.parent = content;
                item.x = 10;
                item.y = h;
                item.width = w-20;
                h = h + item.height;
                //console.log("item:",item.x,item.y,item.width,item.height);
                return h;
            }

            function initialise() {
                for (var i=0; i<items.count; ++i) {
                    items.get(i).clicked.disconnect()
                    items.get(i).clicked.connect(root.clicked)
                    items.get(i).button = true;
                    items.get(i).index = i;
                }
            }

            Component.onCompleted: { initialise(); layout(); }
            onWidthChanged:        { layout(); }
        }
    }

    Component {
        id: line

        Rectangle {
            height: 1
            color: "grey"
        }
    }

    function layout() {
        box.layout();
    }

    onItemsChanged:         box.layout();
    onHeightChanged:        box.layout();
}
