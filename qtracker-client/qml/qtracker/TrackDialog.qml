import QtQuick 1.0

Page {
    id: root
    state: "list"
    signal trackSelected(int trackId)
    signal trackStarted(int trackId)
    signal trackStopped(int trackId)

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }
    function add() {
        state="edit"
    }
    function list() {
        state="list"
    }

    ToolBar {
        id: toolbar

        tools: ToolBarLayout {
            hasRightButton: true
            ToolButton { id: backbutton;    source: "qrc:/images/import.svg";  onClicked: cancel();  }
            ToolButton { id: listbutton;    source: "qrc:/images/options.svg"; onClicked: list();    }
            ToolButton { id: addbutton;     source: "qrc:/images/zoom-in.svg"; onClicked: add();     }
            ToolButton { id: confirmbutton; source: "qrc:/images/visible.svg"; onClicked: confirm(); visible: false }
        }
    }

    TrackList {
        id:tracklist
        width: parent.width
        //height: parent.height-30
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        index: -1
    }

    Rectangle {
        id:newtrack
        width: parent.width
        anchors.top:  toolbar.bottom
        anchors.bottom:  parent.bottom
        color: activePalette.dark
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: activePalette.light
            }
            GradientStop {
                position:  1.0
                color: activePalette.dark
            }
        }

        InputField {
            id: nameinput
            label: "Name: "
            input: "trk-20110328-211000"
        }

        AutoLayout {
            id: dualbox
            anchors.top:    nameinput.bottom
            anchors.bottom: parent.bottom
            anchors.left:   parent.left
            anchors.right:  parent.right

            RadioBox {
                id: base
                visible: true
                onVisibleChanged: dualbox.layoutChildren()
                selected: 0
                radiobuttons: [ RadioButton { text:"All"}, RadioButton{ text:"Time based"}, RadioButton { text:"Distance based"} ]
            }
            RadioBox {
                id: time
                visible: base.selected==1? true: false
                onVisibleChanged: dualbox.layoutChildren()
                selected: 1
                radiobuttons: [ RadioButton { text:"5s"}, RadioButton{ text:"15s"}, RadioButton { text:"1m"}, RadioButton { text:"5m"}, RadioButton { text:"15m"} ]
            }
            RadioBox {
                id: distance
                visible: base.selected==2? true: false
                onVisibleChanged: dualbox.layoutChildren()
                selected: 1
                radiobuttons: [ RadioButton { text:"10m"}, RadioButton{ text:"30m"}, RadioButton { text:"100m"}, RadioButton { text:"300m"}, RadioButton { text:"1km"} ]
            }
        }
    }

    states: [
        State {
            name: "list";
            PropertyChanges { target: confirmbutton; visible: false }
            PropertyChanges { target: tracklist;     visible: true  }
            PropertyChanges { target: newtrack;      visible: false }
        },
        State {
            name: "edit";
            PropertyChanges { target: confirmbutton; visible: true  }
            PropertyChanges { target: tracklist;     visible: false }
            PropertyChanges { target: newtrack;      visible: true  }
        }
    ]

}
