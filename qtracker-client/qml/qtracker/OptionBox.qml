import QtQuick 1.0

Item {}

/* Obsolete

Item {
    id: root
    property alias title: header.text
    property QtObject items: null
    height: header.offset + box.height + 10

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

    Rectangle {
        id: box

        x: 10
        y: 10 + header.offset
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
            console.log("OptionBox.layoutItems(): got", items.count(), "items for model",items.name)
            var h = 2;
            seperators.clear();
            var seperator;
            for (var i=0; i<items.count(); ++i) {
                h = layoutOptionItem(items.get(i),w,h+10) + 10;
                if ((i+1) < items.count()) {
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
            //console.log("OptionBox.layoutOptionItem():",item.x,item.y,item.width,item.height);
            return h;
        }

        onWidthChanged:         layout();
        Component.onCompleted:  layout();
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
*/
