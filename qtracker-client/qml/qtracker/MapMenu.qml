import QtQuick 1.0

Page {
    id: root
    imageSource: "qrc:/images/options-bg.png"

    signal mapSelected(string fileName)

    function cancel() {
        pageStack.pop();
    }

    OptionHeader {
        id: hdr
        x: 0; y:0; width: parent.width; height: 50
        text: "Map"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    OptionList {
        id: edit
        title: "Options"
        x: 0; width: parent.width;
        anchors.top: hdr.bottom;
        items: optionitems

        DynamicItemModel {
            id: optionitems
            name: "optionitems"
            OptionItem { text: "Name" }
            OptionItem { text: "Calibration" }
            OptionItem { text: "Datum" }
        }
    }

    MapList {
        id: lst
        x: 0; width: parent.width;
        anchors.top: edit.bottom;
        anchors.bottom: parent.bottom;
        title: "List"
        onMapSelected: { root.mapSelected(fileName); root.cancel(); }
    }
}
