import QtQuick 1.0

Item {
    id: root
    property alias button: btn.visible
    property alias buttonsource: btn.source
    property bool  hilite: (btn.state=="pressed")
    property color hilitecolor: "#c0c0e0"
    property int   index: -1
    property bool  roundtop: false
    property bool  roundbottom: false

    signal clicked(int index, string name)
    //clip: true
    //smooth: true

    Rectangle {
        id: toprect
        visible: root.hilite
        x: -9;   width: parent.width +20
        y: -11;  height: parent.height +12
        radius: roundtop ? 13 : 0
        color: root.hilitecolor
    }

    Rectangle {
        id: bottomrect
        visible: root.hilite
        x: -9;   width:  parent.width +20
        y: 0;    height: parent.height +12
        radius:  roundbottom ? 13 : 0
        color: root.hilitecolor
    }

    ToolButton {
        id: btn

        x: root.width-txt.height
        y: 0
        width:  txt.height
        height: txt.height

        visible: false
        bgcolor: root.hilitecolor
        source: "forward.svg";
        onClicked: root.clicked(root.index,root.text);
    }

    MouseArea {
        id: mouseArea
        visible: btn.visible
        anchors.fill:  parent
        onClicked:  if (btn.visible) btn.clicked()
        onPressed:  if (btn.visible) btn.pressed()
        onCanceled: if (btn.visible) btn.released()
        onReleased: if (btn.visible) btn.released()
    }
    //onWidthChanged: console.log("item.onWidthChanged", width)
}
