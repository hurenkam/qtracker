import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../components"
import "../main"

OptionPage {
    id: root
    title: "Category List"
    property Item mapview
    rightbutton: true
    rightbuttonsrc: "add-plain.png"
    leftbuttonsrc: "left-plain.png"

    signal categorySelected(int catid)
    signal newCatagory()

    TDatabase {
        id: database
    }

    Component {
        id: delegate
        OptionTextItem {
            id: txt;
            width: parent.width
            text: modelData.name;
            button: true
            index2: index
            onClicked: ListView.view.itemClicked(index)
        }
    }

    Rectangle {
        id: catbox
        anchors.margins: 10
        x:      10
        y:      70
        width:  root.width - 20
        height: root.height - 80
        radius: 12
        color: "white"
        border.color: "grey"
        border.width: 1
        clip: true
        smooth: true
        ListView {
            anchors.margins: 10
            anchors.fill: parent
            id: catlist
            delegate: delegate

            function itemClicked(index) {
                pageStack.pop()
                root.categorySelected(model[index].catid)
            }
        }
    }

    function refreshData() {
        console.log("CategorySelectionPage.refreshData()")
        catlist.model = database.categories
    }

    onRightClicked: {
        pageStack.pop()
        root.newCategory()
    }

    Component.onCompleted: {
        refreshData()
    }
}
