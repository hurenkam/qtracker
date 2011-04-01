import QtQuick 1.0

Page {
    id: root
    signal mapSelected(string fileName)

    function confirm() {
        root.mapSelected(listbox.mapname);
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }

    CalibrationMenu {
        id: calmenu
    }

    function calibration() {
        pageStack.push(calmenu)
    }
/*
    DatumMenu {
        id: calmenu
    }
*/
    function datums() {
        //pageStack.push(calmenu)
    }

    MenuEntry {
        id: heading
        x: 2; y: 2
        width: parent.width
        height: 54
        text: "Map Menu"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
        rightButtonVisible: true
        rightButtonSource: "qrc:/images/visible.svg"
        onRightClicked: root.confirm();
    }

    MenuEntry {
        id: calentry
        anchors.top: heading.bottom
        width: parent.width
        height: 54
        text: "Calibration"
        rightButtonVisible: true
        onRightClicked: root.calibration();
    }

    MenuEntry {
        id: datumentry
        anchors.top: calentry.bottom
        width: parent.width
        height: 54
        text: "Wgs84"
        rightButtonVisible: true
        onRightClicked: root.datums();
    }

    MenuEntry {
        id: list
        anchors.top: datumentry.bottom
        anchors.bottom: parent.bottom
        width: parent.width

        MapList {
            id: listbox
            anchors.fill: parent
        }
    }
}
