import QtQuick 1.0

Item {
    id: root
    property int selected: -1
    property list<Item> radiobuttons
    onSelectedChanged:     layoutButtons()
    onRadiobuttonsChanged: layoutButtons()

    Rectangle {
        id: container
        x: 3
        width: parent.width -6
        y: 3
        height: parent.height -6
        anchors.margins: 5
        radius: 5
        color: Qt.lighter(activePalette.dark)
    }

    MouseArea {
        x: container.x
        y: containet.y
        width: container.width
        height: radiobuttons.length*24

        onClicked: { console.log("MouseArea.onClicked: ", mouseY, (mouseY-12)/24); root.selected=(mouseY-12)/24 }
    }

    function layoutButtons() {
        console.log("RadioBox.layoutButtons: ",root.selected,radiobuttons.length)

        for (var i=0; i<radiobuttons.length; i++) {

            radiobuttons[i].width = width;
            radiobuttons[i].height = 24;
            radiobuttons[i].x = 0;
            radiobuttons[i].y = i*24;
            radiobuttons[i].parent = container
            radiobuttons[i].selected = (i==root.selected)

            console.log("RadioBox.layoutButtons: ",i,radiobuttons[i].selected,radiobuttons[i].x,radiobuttons[i].y,radiobuttons[i].width,radiobuttons[i].height)
        }
    }
}
