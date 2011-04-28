import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root

    signal editRoute(int index)

    Component {
        id: delegate
        OptionTextItem { text: ""; button: true; }
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
        //OptionTextItem { text: "<new>"; button: true; }
    }

    function update() {
        var item = null;
        content.clear();
        item = delegate.createObject(null)
        item.text = "<new>"
        item.button = true;
        content.append(item)
        console.log("routelist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("routelist item ",database.get(i,"name"))
            item = delegate.createObject(null)
            item.text = database.get(i).rteid + " " + database.get(i).name
            content.append(item)
            lst.layout()
        }
    }

    onClicked: {
        console.log("RouteList.onClicked",index,text);
        editRoute(index-1)
    }

    Component.onCompleted: {
        update()
    }
}
