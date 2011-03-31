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
        text: "Stop Track"
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
