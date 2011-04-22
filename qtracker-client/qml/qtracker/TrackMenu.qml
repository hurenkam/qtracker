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

    OptionList {
        id: startstop
        title: "Recording"
        x: 0; width: parent.width;
        anchors.top: hdr.bottom;
        items: recordingitems

        DynamicItemModel {
            id: recordingitems
            name: "recordingitems"

            OptionTextItem { text: trackstatus.status=="idle"? "Start Track": "Stop Track" }
        }
    }

    TrackList {
        id: lst
        x: 0; width: parent.width;
        anchors.top: startstop.bottom;
        anchors.bottom: parent.bottom;
        title: "List"
    }
}
