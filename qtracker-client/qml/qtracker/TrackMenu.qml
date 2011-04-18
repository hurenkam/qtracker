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
        text: "Track"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    ValueSpaceSubscriber  {
        id: trackstatus;
        path: "/server/track/status"
        property string status: value
        onStatusChanged: console.log("TrackMenu.trackstatus:onStatusChanged: ", status)
        Component.onCompleted: console.log("TrackMenu.trackstatus:onCompleted: ", value)
    }

    ValueSpaceSubscriber  {
        id: trackname;
        path: "/server/track/name"
    }

    OptionBox {
        id: startstop
        title: "Recording"
        x: 0; width: parent.width;
        anchors.top: hdr.bottom;
        items: recordingitems

        DynamicItemModel {
            id: recordingitems
            OptionItem { text: trackstatus.status=="idle"? "Start Track": "Stop Track" }
        }
    }

    OptionList {
        id: lst
        x: 0; width: parent.width;
        anchors.top: startstop.bottom;
        anchors.bottom: parent.bottom;
        title: "List"

        Component {
            id: delegate
            OptionItem { text: "" }
        }

        Database {
            id: database
            table: "tracks"
            onCountChanged: lst.update()
        }

        items: content

        DynamicItemModel {
            id: content
        }

        function update() {
            var item = null;
            content.clear();
            console.log("tracklist contains",database.count,"items")
            for (var i=0; i<database.count; i++) {
                console.log("tracklist item ",database.get(i,"name"))
                item = delegate.createObject(null)
                item.text = database.get(i,"trackid") + " " + database.get(i,"name")
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
import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

Page {
    id: root

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }

    function newentry() {
        pageStack.push(edit)
    }

    function stop() {
        console.log("TrackMenu.stop()")
        var cmd = { "class": "server", "method": "trackstop", "args": [ ] }
        client.sendCommand(cmd);
    }

    MenuEntry {
        id: heading
        x: 2; y: 2
        width: parent.width
        height: 54
        text: "Track Menu"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    TrackEdit             { id: edit }
    ValueSpaceSubscriber  {
        id: trackstatus;
        path: "/server/track/status"
        property string status: value
        onStatusChanged: console.log("TrackMenu.trackstatus:onStatusChanged: ", status)
        Component.onCompleted: console.log("TrackMenu.trackstatus:onCompleted: ", value)
    }
    ValueSpaceSubscriber  {
        id: trackname;
        path: "/server/track/name"
    }
    MenuEntry {
        id: start
        visible: trackstatus.status=="idle"? true: false
        anchors.top: heading.bottom
        width: parent.width
        height: 54
        text: "Start Track"
        rightButtonVisible: true
        onRightClicked: root.newentry();
    }

    MenuEntry {
        id: stop
        visible: trackstatus.status=="recording"? true: false
        anchors.top: heading.bottom
        width: parent.width
        height: 54
        //text: "Stop Track"
        text: "Stop " + trackname.value
        rightButtonVisible: true
        onRightClicked: root.stop();
    }

    MenuEntry {
        id: list
        anchors.top: start.bottom
        anchors.bottom: parent.bottom
        width: parent.width

        TrackList {
            id: listbox
            anchors.fill: parent
        }
    }
}
*/
