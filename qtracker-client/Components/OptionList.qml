import QtQuick 1.0

Item {
    id: root
    property alias title: header.text
    property QtObject items: null
    height: header.offset + box.height + 20

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
        height: root.height -header.offset-18
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
            clip: true
            smooth: true
            Item {
                width: box.width
                clip: true
                smooth: true
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
                if (items && (items.count() > 0)) {
                    //items.parent = content
                    content.height = layoutItems(box.width);
                }
                else
                {
                    content.height = 30
                }
            }

            function layoutItems(w) {
                console.log("OptionList.layoutItems(): got", items.count(), "items for model",items.name)
                var h = 2;
                seperators.clear();
                var seperator;
                for (var i=0; i<items.count(); ++i) {
                    var item = items.get(i)
                    item.roundtop = false
                    item.roundbottom = false
                    h = layoutOptionItem(item,w,h+10) + 10;
                    if ((i+1) < items.count()) {
                        seperator = line.createObject(seperators)
                        h = h+1
                        seperator.x = 0
                        seperator.width = w
                        seperator.y = h
                    }
                }
                items.get(0).roundtop = true
                items.get(items.count()-1).roundbottom = true
                return h+2;
            }

            function layoutOptionItem(item,w,h) {
                item.parent = content;
                item.x = 10;
                item.y = h;
                item.width = w-20;
                h = h + item.height;
                //console.log("OptionList.layoutOptionItem():",item.x,item.y,item.width,item.height);
                return h;
            }

            function initialise() {
                console.log("OptionList.initialise():",items.name,items.count());
                for (var i=0; i<items.count(); ++i) {
                    items.get(i).clicked.disconnect(root.clicked)
                    items.get(i).clicked.connect(root.clicked)
                    items.get(i).index = i;
                }
            }

            function connectitem(i) {
                items.get(i).clicked.connect(root.clicked)
                items.get(i).index = i;
            }

            Connections {
                target: items
                onListCleared:     { box.layout(); }
                onListAppended:    { box.connectitem(items.count()-1); }
            }

            Component.onCompleted: { box.layout(); }
            onWidthChanged:        { box.layout(); }
        }
    }

    Component {
        id: line

        Rectangle {
            height: 1
            color: "grey"
        }
    }

    function initialise() {
        box.initialise()
    }

    function layout() {
        box.layout();
    }

    onItemsChanged:         { initialise(); layout(); }
    onHeightChanged:        layout();
}
