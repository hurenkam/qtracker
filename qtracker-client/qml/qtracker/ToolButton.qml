import QtQuick 1.0

Item {
    id: root
    width: parent.height*0.7
    height: parent.height*0.7
    smooth: true
    state: "released"
    property int interval: 0
    property alias selected: bg.visible
    property alias source: img.source
    property alias bgcolor: bg.color

    Rectangle {
        id: bg
        visible: false
        x: 1; y:1; width: parent.width-2; height: parent.height-2
        radius: height/2
        color: "white"
    }

    Image {
        id: img
        visible: true
        anchors.fill: parent
    }

    signal clicked
    signal repeat

    Timer {
        id: timer
        interval: 250; running: false; repeat: true
        onTriggered: root.repeat()
    }

    states: [
        State { name: "pressed";  PropertyChanges { target: root; scale: 0.9 } },
        State { name: "released"; }
    ]

    function pressed() {
        state = "pressed"
        if (repeat) {
            //root.repeat()
            timer.interval = root.interval
            timer.start()
        }
    }

    function released() {
        state = "released"
        if (repeat) {
            timer.stop()
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill:  parent
        onClicked: root.clicked();
        onPressed: root.pressed()
        onCanceled: root.released()
        onReleased: root.released()
    }
}
