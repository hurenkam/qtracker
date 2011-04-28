import QtQuick 1.0

Item {
    id: root

    property alias leftButtonVisible:  leftbutton.visible
    property alias leftButtonSource:   leftbutton.source
    property alias rightButtonVisible: rightbutton.visible
    property alias rightButtonSource:  rightbutton.source
    property alias text:               label.text
    property real  buttonheight:       50

    signal leftClicked()
    signal rightClicked()

    Rectangle {
        id: rect
        anchors.fill: parent
        color: activePalette.light
        state: "normal"

        property QtObject normalgradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.lighter(activePalette.light)
            }
            GradientStop {
                position:  1.0
                color: Qt.lighter(activePalette.dark)
            }
        }
        property QtObject pressedgradient: Gradient {
            GradientStop {
                position: 0.0
                color: activePalette.light
            }
            GradientStop {
                position:  1.0
                color: activePalette.dark
            }
        }
        gradient: state=="pressed"? pressedgradient : normalgradient

        ToolButton {
            id: leftbutton

            x: root.buttonheight * 0.1
            y: root.buttonheight * 0.1
            width:  root.buttonheight *0.8
            height: root.buttonheight *0.8

            visible: false
            bgcolor: "black"
            source: "back.svg";
            onClicked: root.leftClicked();
        }

        Text {
            id: label
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -5
            text: ""
            font.pixelSize: parent.height/2
            color: "white"
            visible: text!=""? true: false
        }

        ToolButton {
            id: rightbutton

            x: parent.width - root.buttonheight * 1.1
            y: root.buttonheight * 0.1
            width:  root.buttonheight *0.8
            height: root.buttonheight *0.8

            visible: false
            source: "forward.svg";
            bgcolor: "black"
            onClicked: root.rightClicked();
        }

        function clicked() {
            console.log("MenuEntry.clicked")
            if (leftbutton.visible && !rightbutton.visible) root.leftClicked()
            if (!leftbutton.visible && rightbutton.visible) root.rightClicked()
        }
        function pressed() {
            console.log("MenuEntry.pressed")
            if (leftbutton.visible && !rightbutton.visible) state = "pressed"
            if (!leftbutton.visible && rightbutton.visible) state = "pressed"
        }
        function released() {
            console.log("MenuEntry.released")
            state = "normal"
        }

        MouseArea {
            id: leftarea
            anchors.left: parent.left
            width: parent.width/2
            height: parent.height
            onClicked: root.leftClicked()
            onPressed:  { rect.state="pressed"; leftbutton.pressed();   }
            onCanceled: { rect.state="normal";  leftbutton.cancelled(); }
            onReleased: { rect.state="normal";  leftbutton.released();  }
        }
        MouseArea {
            id: rightarea
            anchors.right: parent.right
            height: parent.height
            width: parent.width/2
            onClicked: root.rightClicked()
            onPressed:  { rect.state="pressed"; rightbutton.pressed();   }
            onCanceled: { rect.state="normal";  rightbutton.cancelled(); }
            onReleased: { rect.state="normal";  rightbutton.released();  }
        }
    }
}
