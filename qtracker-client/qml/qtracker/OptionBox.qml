import QtQuick 1.0

Item {
    id: root
    property alias title: header.text
    property QtObject items: null
    height: box.height + header.offset + 4

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

    Rectangle {
        id: box

        x: 10
        y: 10 + header.offset
        width: parent.width -20
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
                //items.parent = box;
                //items.x = 0;
                //items.y = 0;
                //items.width = box.width;
                layoutItems(box.width);
                //items.height = box.height;
                content.x = 0;
                content.y = 0;
                content.width = box.width;
                content.height = box.height;
            }
        }

        function layoutItems(w) {
            //console.log("got", items.children.length, "items")
            var h = 2;
            seperators.clear();
            var seperator;
            for (var i=0; i<items.children.length; ++i) {
                h = layoutOptionItem(items.children[i],w,h+10) + 10;
                if ((i+1) < items.children.length) {
                    seperator = line.createObject(seperators)
                    h = h+1
                    seperator.y = h
                }
            }
            box.height = h+2;
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

        //Component.onCompleted: console.log("optionbox.oncompleted:    width = ",box.width)
        //onWidthChanged:        console.log("optionbox.onwidthchanged: width = ",box.width)
        onWidthChanged:         layout();
        Component.onCompleted:  layout();
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
        box.layout();
    }

    onItemsChanged:         box.layout();
    onHeightChanged:        box.layout();
}
