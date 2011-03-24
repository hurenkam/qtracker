import QtQuick 1.0

Image {
    id: root
    width: 50
    height: 50
    smooth: true
    state: "released"
    property int interval: 0

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
