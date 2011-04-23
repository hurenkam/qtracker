import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root

    Component {
        id: delegate
        OptionTextItem { text: "" }
    }

    Database {
        id: database
        table: "categories"
        onCountChanged: lst.update()
    }

    items: content

    DynamicItemModel {
        id: content
        name: "CategoryList"
    }

    function update() {
        var item = null;
        content.clear();
        console.log("categorylist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("categorylist item ",database.get(i,"name"))
            item = delegate.createObject(null)
            item.text = database.get(i,"categoryid") + " " + database.get(i,"name")
            content.append(item)
            lst.layout()
        }
    }

    onClicked: {
        console.log("CategoryList.onClicked",index,text);
    }

    Component.onCompleted: update()
}
