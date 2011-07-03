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
    title: "Trip"
    imageSource: "../Images/options-bg.png"
    rightbutton: true
    rightbuttonsrc: "../Images/options-plain.svg"
    rightbuttonradius: 0
    leftbutton: true
    leftbuttonsrc: "../Images/left-plain.svg"
    leftbuttonradius: 0
    property int tripid: 1
    property TTrip dbrecord

    tools: ToolBarLayout {
        id: maintools
        ToolButton { id: okbutton;   source: "../Images/visible-plain.svg";   bgcolor: "black"; bgradius: 0 }
        ToolButton { visible: false; showbg: false }
        ToolButton { visible: false; showbg: false }
        ToolButton { id: wptbutton;  source: "../Images/flag-plain.svg";      bgcolor: "black"; bgradius: 0; onClicked: wptShowList() }
        ToolButton { id: rtebutton;  source: "../Images/route-plain.svg";     bgcolor: "black"; bgradius: 0; onClicked: rteShowList() }
        ToolButton { id: trkbutton;  source: "../Images/hiker-plain.svg";     bgcolor: "black"; bgradius: 0; onClicked: trkShowList() }
        height: 60
    }

    ValueSpaceSubscriber  {
        id: vcurrenttrip;
        path: "/server/trip/id"
        property int tripid: value
    }

    ValueSpaceSubscriber  {
        id: vcurrentname;
        path: "/server/trip/name"
        property string name: value
    }

    ValueSpaceSubscriber {
        id: vtriptime
        path: "/server/time/trip"
        property int hour:   value? value.hour   : 0
        property int minute: value? value.minute : 0
        property int second: value? value.second : 0
        property string text: hour.toString() + ":" + minute.toString() + "." + second.toString()
        //property string text: Qt.formatTime(value,"hh:mm:ss")
    }
    ValueSpaceSubscriber { id: vdistance; path: "/server/location/distance"; property double d: value }
    ValueSpaceSubscriber { id: vspeedmin; path: "/server/speed/min";         property double d: value }
    ValueSpaceSubscriber { id: vspeedmax; path: "/server/speed/max";         property double d: value }
    ValueSpaceSubscriber { id: vspeedavg; path: "/server/speed/average";     property double d: value }
    ValueSpaceSubscriber { id: valtmin;   path: "/server/altitude/min";      property double d: value }
    ValueSpaceSubscriber { id: valtmax;   path: "/server/altitude/max";      property double d: value }
    ValueSpaceSubscriber { id: valtavg;   path: "/server/altitude/average";  property double d: value }
    ValueSpaceSubscriber { id: vascent;   path: "/server/altitude/ascent";   property double d: value }
    ValueSpaceSubscriber { id: vdescent;  path: "/server/altitude/descent";  property double d: value }

    QtObject {
        id: record
        property bool current: ((vcurrenttrip.tripid == root.tripid) || !dbrecord)
        //property bool current: true
        property int     tripid:   (current)? vcurrenttrip.tripid : root.tripid
        property string  name:     (current)? vcurrentname.text   : dbrecord.name
        property string  triptime: (current)? vtriptime.text  : Qt.formatTime(dbrecord.triptime,"hh:mm:ss")
        property string  tripdist: (current)? vdistance.d     : dbrecord.tripdist
        property double  speedavg: (current)? vspeedavg.d     : dbrecord.speedavg
        property double  speedmin: (current)? vspeedmin.d     : dbrecord.speedmin
        property double  speedmax: (current)? vspeedmax.d     : dbrecord.speedmax
        property double  ascent:   (current)? vascent.d       : dbrecord.ascent
        property double  descent:  (current)? vdescent.d      : dbrecord.descent
        property double  altavg:   (current)? valtavg.d       : dbrecord.altavg
        property double  altmin:   (current)? valtmin.d       : dbrecord.altmin
        property double  altmax:   (current)? valtmax.d       : dbrecord.altmax
    }

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
            onClicked: ListView.view.itemClicked(index)
        }
    }

    OptionList {
        id: tripdata
        title: "Trip Data"
        items: tripitems
        x:      0
        y:      50
        width:  root.width
        height: 170

        DynamicItemModel {
            id: tripitems
            name: "tripitems"

            OptionInputItem { id: tripname;     text: "Name:     "; value: record.name;     onValueChanged: settings.setProperty("trip_defaultname",value) }
            OptionInputItem { id: triptime;     text: "Time:     "; value: record.triptime; readOnly: true }
            OptionInputItem { id: tripdist;     text: "Distance: "; value: record.tripdist; readOnly: true }
        }
    }

    OptionList {
        id: speeddata
        title: "Speed Data"
        items: speeditems
        x:      0
        y:      200
        width:  root.width
        height: 170

        DynamicItemModel {
            id: speeditems
            name: "speeditems"

            OptionInputItem { id: tripavgspeed; text: "Average:  "; value: record.speedavg; readOnly: true }
            OptionInputItem { id: tripminspeed; text: "Minimum:  "; value: record.speedmin; readOnly: true }
            OptionInputItem { id: tripmaxspeed; text: "Maximum:  "; value: record.speedmax; readOnly: true }
        }
    }

    OptionList {
        id: altdata
        title: "Altitude Data"
        items: altitems
        x:      0
        y:      350
        width:  root.width
        height: 270

        DynamicItemModel {
            id: altitems
            name: "altitems"

            OptionInputItem { id: tripascent;   text: "Ascent:   "; value: record.ascent;   readOnly: true }
            OptionInputItem { id: tripdescent;  text: "Descent:  "; value: record.descent;  readOnly: true }
            OptionInputItem { id: tripavgalt;   text: "Average:  "; value: record.altavg;   readOnly: true }
            OptionInputItem { id: tripminalt;   text: "Minimum:  "; value: record.altmin;   readOnly: true }
            OptionInputItem { id: tripmaxalt;   text: "Maximum:  "; value: record.altmax;   readOnly: true }
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
        onRightClicked: { root.rteAdd()    }
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
        onRightClicked: { root.trkAdd()    }
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

    TripSelectionPage { id: tripSelectPage; onTripSelected: root.tripid = tripid; }
    WaypointEditPage  { id: wptedit; }


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

    function trkAdd() {
    }

    function trkSelected(id) {
    }

    function refreshData() {
        console.log("TripEditPage.refreshData(",root.tripid,")")
        dbrecord = database.getTrip(root.tripid)
        console.log("dbrecord: ",dbrecord.tripid, dbrecord.name)

        dbrecord.selectWaypoints(0,50)
        wptlist.model = dbrecord.waypoints
        dbrecord.selectRoutes(0,50)
        rtelist.model = dbrecord.routes
        dbrecord.selectTracks(0,50)
        trklist.model = dbrecord.tracks
    }

    function selectTrip() {
        pageStack.push(tripSelectPage)
    }

    onRightClicked: selectTrip()
    onTripidChanged: refreshData()
    Component.onCompleted: refreshData()
}
