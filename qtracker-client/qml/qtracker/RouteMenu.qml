import QtQuick 1.0

import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

Page {
    id: root
    imageSource: "qrc:/images/options-bg.png"

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }

    OptionHeader {
        id: hdr
        x: 0; y:0; width: parent.width; height: 50
        text: "Route"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    OptionList {
        id: editroute
        title: "Edit"
        x: 0; width: parent.width;
        anchors.top: hdr.bottom;
        items: editrouteitems

        DynamicItemModel {
            id: editrouteitems
            name: "editrouteitems"

            OptionItem { text: "Name" }
            OptionItem { text: "Route points" }
        }
    }

    OptionList {
        id: lst
        x: 0; width: parent.width;
        anchors.top: editroute.bottom;
        anchors.bottom: parent.bottom;
        title: "List"

        Component {
            id: delegate
            OptionItem { text: "" }
        }

        Database {
            id: database
            table: "routes"
            onCountChanged: lst.update()
        }

        items: content

        DynamicItemModel {
            id: content
            name: "RouteList"
        }

        function update() {
            var item = null;
            content.clear();
            console.log("routelist contains",database.count,"items")
            for (var i=0; i<database.count; i++) {
                console.log("routelist item ",database.get(i,"name"))
                item = delegate.createObject(null)
                item.text = database.get(i,"routeid") + " " + database.get(i,"name")
                content.append(item)
                lst.layout()
            }
        }

        onClicked: {
            console.log("TrackMenu.onClicked",index,text);
        }

        Component.onCompleted: update()
    }
}

/*
Page {
    id: root

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }
/ *
    RouteEdit {
        id: edit
    }
* /
    function newentry() {
        //pageStack.push(edit)
    }

    MenuEntry {
        id: heading
        x: 2; y: 2
        width: parent.width
        height: 54
        text: "Route Menu"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    MenuEntry {
        id: start
        anchors.top: heading.bottom
        width: parent.width
        height: 54
        text: "New Route"
        rightButtonVisible: true
        onRightClicked: root.newentry();
    }

    MenuEntry {
        id: list
        anchors.top: start.bottom
        anchors.bottom: parent.bottom
        width: parent.width
/ *
        WaypointList {
            id: list
            anchors.fill: parent
        }
* /
    }
}
*/
