import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import QmlTrackerExtensions 1.0
import "../Components"

OptionPage {
    id: root
    title: "Refpoint List"
    property int mapid: -1
    property TMap dbrecord
    leftbuttonsrc: "../Images/left-plain.svg"
    rightbutton: true
    rightbuttonsrc: "../Images/add-plain.svg"

    RefpointEditPage {
        id: refpointEditPage
    }

    TDatabase {
        id: database
    }

    Component {
        id: delegate
        OptionTextItem {
            id: txt;
            width: parent? parent.width : 0
            text: modelData.name;
            button: true
            onClicked: ListView.view.itemClicked(index)
        }
    }

    Rectangle {
        id: refbox
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
            id: reflist
            delegate: delegate

            function itemClicked(index) {
                refpointEditPage.refid = model[index].refid
                pageStack.push(refpointEditPage)
            }
        }
    }

    function refreshData() {
        console.log("RefpointEditPage.refreshData(",root.mapid,")")
        dbrecord = database.getMap(root.mapid)
        dbrecord.selectRefpoints(0,15)
        reflist.model = dbrecord.refpoints
        console.log("dbrecord: ",dbrecord.mapid, dbrecord.name)
    }

    onMapidChanged: refreshData()
    Component.onCompleted: refreshData()
}
