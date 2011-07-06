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
    title: "Category"
    imageSource: "../Images/options-bg.png"
    rightbutton: true
    rightbuttonsrc: "../Images/options-plain.svg"
    rightbuttonradius: 0
    leftbutton: true
    leftbuttonsrc: "../Images/left-plain.svg"
    leftbuttonradius: 0
    property int catid: 1
    property TCategory dbrecord

    tools: ToolBarLayout {
        id: maintools
        ToolButton { id: okbutton;     source: "../Images/visible-plain.svg";   bgcolor: "black"; bgradius: 0 }
        ToolButton { id: exportbutton; source: "../Images/export-plain.svg";    bgcolor: "black"; bgradius: 0; onClicked: gpxExport()   }
        ToolButton { visible: false;   showbg: false; }
        ToolButton { id: wptbutton;    source: "../Images/flag-plain.svg";      bgcolor: "black"; bgradius: 0; onClicked: wptShowList() }
        ToolButton { id: rtebutton;    source: "../Images/route-plain.svg";     bgcolor: "black"; bgradius: 0; onClicked: rteShowList() }
        ToolButton { id: trkbutton;    source: "../Images/hiker-plain.svg";     bgcolor: "black"; bgradius: 0; onClicked: trkShowList() }
        height: 60
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
            index2: index
            onClicked: ListView.view.itemClicked(index)
        }
    }

    OptionList {
        id: catdata
        title: "Category Data"
        items: catitems
        x:      0
        y:      50
        width:  root.width
        height: 170

        DynamicItemModel {
            id: catitems
            name: "catitems"

            OptionInputItem { id: catname; text: "Name: "; value: dbrecord.name; onValueChanged: settings.setProperty("cat_defaultname",value) }
        }
    }

    OptionSubPage {
        id: wptpage
        title: "Waypoints"
        z: 20
        y: root.height
        Behavior on y {
            NumberAnimation { easing.type: Easing.InOutQuart; duration: 300 }
        }
        visible: false
        onRightClicked: { root.wptAdd()    }
        onLeftClicked:  { y = root.height; }

        Rectangle {
            id: wptbox
            anchors.margins: 10
            x: 10
            y: 70
            width: parent.width - 20
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
                id: wptlist
                delegate: delegate

                function itemClicked(index) {
                    root.wptSelected(model[index].wptid)
                }
            }
        }
    }

    OptionSubPage {
        id: rtepage
        title: "Routes"
        z: 20
        y: root.height
        Behavior on y {
            NumberAnimation { easing.type: Easing.InOutQuart; duration: 300 }
        }
        visible: false
        onRightClicked: { root.rteAdd();   }
        onLeftClicked:  { y = root.height; }

        Rectangle {
            id: rtebox
            anchors.margins: 10
            x: 10
            y: 70
            width: parent.width - 20
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
                id: rtelist
                delegate: delegate

                function itemClicked(index) {
                    root.rteSelected(model[index].rteid)
                }
            }
        }
    }

    OptionSubPage {
        id: trkpage
        title: "Tracks"
        z: 20
        y: root.height
        Behavior on y {
            NumberAnimation { easing.type: Easing.InOutQuart; duration: 300 }
        }
        visible: false
        onRightClicked: { root.trkStart(); }
        onLeftClicked:  { y = root.height; }

        Rectangle {
            id: trkbox
            anchors.margins: 10
            x: 10
            y: 70
            width: parent.width - 20
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
                id: trklist
                delegate: delegate

                function itemClicked(index) {
                    root.trkSelected(model[index].trkid)
                }
            }
        }
    }

    CategorySelectionPage  { id: catSelectPage; onCategorySelected: root.catid = catid; }
    WaypointEditPage       { id: wptedit; }
    TrackRecordingPage     { id: trkedit;        onTrackSaved:    trkSaved(trkid);      }

    function wptShowList() {
        root.refreshData()
        wptpage.visible = true;
        wptpage.y = 0
    }

    function wptAdd() {
        wptedit.wptid = -1
        pageStack.push(wptedit)
    }

    function wptSelected(id) {
        wptedit.wptid = id
        pageStack.push(wptedit)
    }

    function rteShowList() {
        rtepage.visible = true;
        rtepage.y = 0
    }

    function rteAdd() {
    }

    function rteSelected(id) {
    }

    function trkShowList() {
        trkpage.visible = true;
        trkpage.y = 0
    }

    function trkStart() {
        trkedit.trkid = -1
        pageStack.push(trkedit)
    }

    function trkStop() {
    }

    function trkSelected(id) {
        trkedit.trkid = id
        pageStack.push(trkedit)
    }

    function trkSaved(id) {
    }

    function gpxExport() {
    }

    function refreshData() {
        console.log("CategoryEditPage.refreshData(",root.catid,")")
        dbrecord = database.getCategory(root.catid)
        console.log("dbrecord: ",dbrecord.tripid, dbrecord.name)

        dbrecord.selectWaypoints(0,50)
        wptlist.model = dbrecord.waypoints
        dbrecord.selectRoutes(0,50)
        rtelist.model = dbrecord.routes
        dbrecord.selectTracks(0,50)
        trklist.model = dbrecord.tracks
    }

    function selectCategory() {
        pageStack.push(catSelectPage)
    }

    onRightClicked: selectCategory()
    onCatidChanged: refreshData()
    Component.onCompleted: refreshData()
}
