import QtQuick 1.0

Page {
    id: root

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }
/*
    RefpointEdit {
        id: edit
    }
*/
    function newentry() {
        //pageStack.push(edit)
    }

    MenuEntry {
        id: heading
        x: 2; y: 2
        width: parent.width
        height: 54
        text: "Calibration Menu"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    MenuEntry {
        id: start
        anchors.top: heading.bottom
        width: parent.width
        height: 54
        text: "New Refpoint"
        rightButtonVisible: true
        onRightClicked: root.newentry();
    }

    MenuEntry {
        id: list
        anchors.top: start.bottom
        anchors.bottom: parent.bottom
        width: parent.width
/*
        RefpointList {
            id: listbox
            anchors.fill: parent
        }
*/
    }
}
