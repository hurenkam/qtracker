import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import QmlTrackerExtensions 1.0
import "../Components"
import "../Main"
import "../Waypoint"
import "../Route"
import "../Track"

OptionPage {
    id: root
    title: "Map List"
    leftbuttonsrc: "../Images/left-plain.svg"

    signal mapSelected(int mapid)

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
        id: mapbox
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
            id: maplist
            delegate: delegate

            function itemClicked(index) {
                pageStack.pop()
                root.mapSelected(model[index].mapid)
            }
        }
    }

    function refreshData() {
        console.log("MapSelectionPage.refreshData()")
        maplist.model = database.maps
    }

    Component.onCompleted: refreshData()
}
