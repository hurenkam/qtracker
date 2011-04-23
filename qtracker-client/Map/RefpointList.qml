import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root

    signal editRefpoint(int index)

    Component {
        id: delegate
        OptionTextItem { text: "" }
    }

    Database {
        id: database
        table: "refpoints"
        onCountChanged: lst.update()
    }

    items: content

    DynamicItemModel {
        id: content
        name: "RefpointList"
        OptionTextItem { text: "<new>"; button: true; }
    }

    function update() {
        var item = null;
        content.clear();
        console.log("refpointlist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("refpointlist item ",database.get(i,"name"))
            item = delegate.createObject(null)
            item.text = database.get(i,"refpointid") + " " + database.get(i,"name")
            content.append(item)
            lst.layout()
        }
    }

    onClicked: {
        console.log("RefpointList.onClicked",index,text);
        editRefpoint(index-1)
    }

    Component.onCompleted: update()
}
