import QtQuick 1.0

Item {
    id: root
    property alias text: txt.text
    property alias button: btn.visible
    property int index: -1
    height: txt.height

    signal clicked(int index, string name)

    Text {
        id: txt
        color: "black"
        font.bold: true
        text: "item"
    }

    ToolButton {
        id: btn

        x: root.width-txt.height
        y: 0
        width:  txt.height
        height: txt.height

        visible: false
        source: "qrc:/images/forward.svg";
        onClicked: root.clicked(root.index,root.text);
    }

    //onWidthChanged: console.log("item.onWidthChanged", width)
}
