import QtQuick 1.0

OptionItem {
    id: root
    property alias text: txt.text
    property alias value: inputfield.text
    property alias readOnly: inputfield.readOnly
    height: txt.height
    signal accepted(int index,string value)

    Text {
        id: txt
        color: "black"
        font.bold: true
        text: ""
    }

    Rectangle {
        id: inputrect
        color: "black"
        visible: root.button? false : inputfield.focus
        x: txt.width + 10
        width: parent.width - txt.width - root.height -10
        y: -2
        height: txt.height+4
    }

    TextInput {
        id: inputfield
        focus: false
        activeFocusOnPress: (!root.button)
        x: root.width - width - root.height - 5
        y: txt.height - height
        font.pointSize: root.height/5
        text: "<content>"
        color: focus? "white" : "blue"
        onAccepted: {
            focus = false
            root.accepted(root.index,root.value)
        }
    }

    MouseArea {
        id: mouseArea
        visible: (!root.button)
        anchors.fill: parent
        onClicked: inputfield.focus = true
    }

    onAccepted: console.log("OptionInputItem.onAccepted",index,value)
}
