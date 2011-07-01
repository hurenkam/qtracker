import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import QmlTrackerExtensions 1.0
import "../Components"
import "../Gauges"
import "../Map"
import "../Waypoint"
import "../Route"
import "../Track"
import "../Trip"

TabOptionPage {
    id: root
    title: "Category"
    imageSource: "../Images/options-bg.png"
    rightbutton: true
    rightbuttonsrc: "../Images/options.svg"
    allowalldown: false
    property int catid: 1
    property TCategory dbrecord

    function wptSelected(id) {
        wptedit.wptid = id
        pageStack.push(wptedit);
    }

    function rteSelected(id) {
        var rte = database.getRoute(id)
    }

    function trkSelected(id) {
        var trk = database.getTrack(id)
    }

    TDatabase {
        id: database
    }

    WaypointEditPage {
        id: wptedit;
        onWaypointSaved: lst.saveWaypoint(index,name,lat,lon,alt)
    }

    Component {
        id: delegate
        OptionTextItem {
            id: txt;
            width: parent.width
            text: modelData.name;
            button: true
            onClicked: ListView.view.itemClicked(index)
        }
    }

    OptionList {
        id: catbox
        x:      0
        y:      50
        width:  root.width
        height: root.height - 100
        title: "Category"
        items: catitems

        DynamicItemModel {
            id: catitems
            name: "catitems"

            OptionInputItem { id: catname; text: "Name:"; value: dbrecord.name }
        }
    }

    tabs: TabLayout {
        //anchors.fill: parent
        x:      10
        y:      100
        width:  root.width - 20
        height: root.height - 160
        id: tablayout

        TabItem {
            title: "Waypoints"
            Rectangle {
                id: wptbox
                anchors.margins: 10
                x: 0
                y: 0
                width: parent.width
                height: root.height - 180
                radius: 12
                color: "white"
                border.color: "grey"
                border.width: 1
                clip: true
                smooth: true
                ListView {
                    anchors.margins: 10
                    anchors.fill: parent
                    id: wptlist
                    delegate: delegate

                    function itemClicked(index) {
                        root.wptSelected(model[index].wptid)
                    }
                }
            }
        }
        TabItem {
            title: "Routes"
            Rectangle {
                id: rtebox
                anchors.margins: 10
                x: 0
                y: 0
                width: parent.width
                height: root.height - 180
                radius: 12
                color: "white"
                border.color: "grey"
                border.width: 1
                clip: true
                smooth: true
                ListView {
                    anchors.margins: 10
                    anchors.fill: parent
                    id: rtelist
                    delegate: delegate

                    function itemClicked(index) {
                        root.rteSelected(model[index].rteid)
                    }
                }
            }
        }
        TabItem {
            title: "Tracks"
            Rectangle {
                id: trkbox
                anchors.margins: 10
                x: 0
                y: 0
                width: parent.width
                height: root.height - 180
                radius: 12
                color: "white"
                border.color: "grey"
                border.width: 1
                clip: true
                smooth: true
                ListView {
                    anchors.margins: 10
                    anchors.fill: parent
                    id: trklist
                    delegate: delegate

                    function itemClicked(index) {
                        root.trkSelected(model[index].trkid)
                    }
                }
            }
        }
    }

    function refreshData() {
        console.log("CategoryEditPage.refreshData(",root.catid,")")
        dbrecord = database.getCategory(root.catid)
        console.log("dbrecord: ",dbrecord.catid, dbrecord.name)

        dbrecord.selectWaypoints(0,100)
        wptlist.model = dbrecord.waypoints
        dbrecord.selectRoutes(0,100)
        rtelist.model = dbrecord.routes
        dbrecord.selectTracks(0,100)
        trklist.model = dbrecord.tracks
    }

    CategorySelectionPage { id: catSelectPage; onCategorySelected: root.catid = catid; }
    function selectCategory() {
        pageStack.push(catSelectPage)
    }

    onRightClicked: selectCategory()
    onCatidChanged: refreshData()
    Component.onCompleted: refreshData()
}
