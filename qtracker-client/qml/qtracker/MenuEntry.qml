import QtQuick 1.0

Item {}

/* Obsolete

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

        x: 2
        y: 2
        width: parent.width -4
        height: parent.height -4
        radius: 15
        color: activePalette.light
        state: "normal"
        //property color gradientstart: activePalette.light
        //property color gradientstop:  activePalette.dark

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
            source: "qrc:/images/back.svg";
            onClicked: root.leftClicked();
        }

        Text {
            id: label
            x: root.buttonheight * 1.2;
            y: root.buttonheight * 0.1
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
            source: "qrc:/images/forward.svg";
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
            anchors.left:  leftbutton.visible?  leftbutton.right : parent.left
            anchors.right: rightbutton.visible? rightbutton.left : parent.right
            height: parent.height
            onClicked: parent.clicked()
            onPressed: parent.pressed()
            onCanceled: parent.released()
            onReleased: parent.released()
        }
    }
}
*/
