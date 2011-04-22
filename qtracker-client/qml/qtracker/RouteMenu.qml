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

            OptionTextItem { text: "Name" }
            OptionTextItem { text: "Route points" }
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
            OptionTextItem { text: "" }
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
