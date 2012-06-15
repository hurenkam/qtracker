import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../Components"
import "../Main"
import "../Waypoint"
import "../Route"
import "../Track"

OptionPage {
    id: root
    title: "Trip List"
    property Item mapview
    leftbuttonsrc: "../Images/left-plain.png"

    signal tripSelected(int tripid)

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
            onClicked: {
                console.log("TripSelectionPage.delegate ",index)
                ListView.view.itemClicked(index)
            }
        }
    }

    Rectangle {
        id: tripbox
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
            id: triplist
            delegate: delegate

            function itemClicked(index) {
                console.log("TripSelectionPage.triplist.itemClicked(",index,")")
                pageStack.pop()
                root.tripSelected(model[index].tripid)
            }
        }
    }

    function refreshData() {
        console.log("TripSelectionPage.refreshData()")
        triplist.model = database.trips
    }

    Component.onCompleted: refreshData()
}
