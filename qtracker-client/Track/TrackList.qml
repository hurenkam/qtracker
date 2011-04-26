import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionList {
    id: root

    signal recordTrack(int index)

    Component {
        id: delegate
        OptionTextItem { text: "" }
    }

    Database {
        id: database
        table: "tracks"
        onCountChanged: lst.update()
    }

    items: content

    DynamicItemModel {
        id: content
        name: "TrackList"
        OptionTextItem { text: "<new>"; button: true; }
    }

    function update() {
        var item = null;
        content.clear();
        console.log("tracklist contains",database.count,"items")
        for (var i=0; i<database.count; i++) {
            console.log("tracklist item ",database.get(i).name)
            item = delegate.createObject(null)
            item.text = database.get(i).trackid + " " + database.get(i).name
            content.append(item)
            lst.layout()
        }
    }

    onClicked: {
        console.log("TrackList.onClicked",index,text);
        recordTrack(index-1)
    }

    Component.onCompleted: update()
}
