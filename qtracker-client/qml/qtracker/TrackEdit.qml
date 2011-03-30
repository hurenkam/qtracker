import QtQuick 1.0

Page {
    id: root

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }
    function newtrack() {
    }

    Rectangle {
        id: heading

        x: 2
        y: 2
        width: parent.width -4
        height:  50
        radius:  15
        color: activePalette.light
        state: "normal"

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: state=="pressed"? Qt.darker(activePalette.light) : Qt.lighter(activePalette.light)
            }
            GradientStop {
                position:  1.0
                color: state=="pressed"? Qt.darker(activePalette.dark) : Qt.lighter(activePalette.dark)
            }
        }

        ToolButton {
            x: 5
            y: 5
            width: 40
            height: 40

            source: "qrc:/images/back.svg";
            onClicked: root.cancel();
        }

        Text {
            x: 70; y: 5
            text: "New Track"
            font.pixelSize: 25
            color: "white"
            visible: text? true: false
        }

        MouseArea {
            anchors.fill:  parent
            onClicked: root.cancel()
            onPressed: state="pressed"
            onCanceled: state="normal"
            onReleased: state="normal"
        }
    }

    Rectangle {
        id: interval

        x: 2
        y: 56
        width: parent.width -4
        height: parent.height -58
        radius:  15
        color: activePalette.light

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
            x: 5; y:5;
            height: 50
            width: parent.width-10
            id: nameinput
            input: "trk-20110328-211000"
        }
        RadioBox {
            id: base
            x: 5; y:60
            width: landscape? parent.width/2 -10 : parent.width-10
            visible: true
            buttonheight: 45
            selected: 0
            radiobuttons: [ RadioButton { text:"All"}, RadioButton{ text:"Time based"}, RadioButton { text:"Distance based"} ]
        }
        RadioBox {
            id: time
            x: landscape? parent.width/2 + 5 : 5;
            y: landscape? 60 : base.height+70
            width: landscape? parent.width/2 -10 : parent.width-10
            buttonheight: 45
            visible: base.selected==1? true: false
            selected: 1
            radiobuttons: [ RadioButton { text:"5s"}, RadioButton{ text:"15s"}, RadioButton { text:"1m"}, RadioButton { text:"5m"}, RadioButton { text:"15m"} ]
        }
        RadioBox {
            id: distance
            x: landscape? parent.width/2 + 5 : 5;
            y: landscape? 60 : base.height+70
            width: landscape? parent.width/2 -10 : parent.width-10
            buttonheight: 45
            visible: base.selected==2? true: false
            selected: 1
            radiobuttons: [ RadioButton { text:"10m"}, RadioButton{ text:"30m"}, RadioButton { text:"100m"}, RadioButton { text:"300m"}, RadioButton { text:"1km"} ]
        }
    }
}

