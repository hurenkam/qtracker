import QtQuick 1.0
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
    rightbuttonsrc: "../Images/options-plain.png"
    rightbuttonradius: 0
    leftbutton: true
    leftbuttonsrc: "../Images/left-plain.png"
    leftbuttonradius: 0
    property int tripid: server.trip
    property TTrip dbrecord

     tools: ToolBarLayout {
        id: maintools
        ToolButton { id: okbutton;   source: "../Images/visible-plain.png";   bgcolor: "black"; bgradius: 0 }
        ToolButton { visible: false; showbg: false }
        ToolButton { visible: false; showbg: false }
        ToolButton { id: wptbutton;  source: "../Images/flag-plain.png";      bgcolor: "black"; bgradius: 0; onClicked: wptShowList() }
        ToolButton { id: rtebutton;  source: "../Images/route-plain.png";     bgcolor: "black"; bgradius: 0; onClicked: rteShowList() }
        ToolButton { id: trkbutton;  source: "../Images/hiker-plain.png";     bgcolor: "black"; bgradius: 0; onClicked: trkShowList() }
        height: 60
    }

    ServerModel {
        id: server
    }

    AltitudeModel { id: altitude }
    SpeedModel    { id: speed }
    DistanceModel { id: distance }
    TimeModel     { id: time }

    QtObject {
        id: record
        property bool current: ((server.trip == root.tripid) || !dbrecord)
        //property bool current: true
        property int     tripid:   (current)? server.tripid    : root.tripid
        //property string  name:     (current)? server.tripname  : dbrecord.name
        property string  triptime: (current)? Qt.formatTime(time.elapsed,"hh:mm:ss") : Qt.formatTime(dbrecord.triptime,"hh:mm:ss")
        property string  tripdist: (current)? distance.current : dbrecord.tripdist
        property double  speedavg: (current)? speed.average    : dbrecord.speedavg
        property double  speedmin: (current)? speed.minimum    : dbrecord.speedmin
        property double  speedmax: (current)? speed.maximum    : dbrecord.speedmax
        property double  ascent:   (current)? altitude.ascent  : dbrecord.ascent
        property double  descent:  (current)? altitude.descent : dbrecord.descent
        property double  altavg:   (current)? altitude.average : dbrecord.altavg
        property double  altmin:   (current)? altitude.minimum : dbrecord.altmin
        property double  altmax:   (current)? altitude.maximum : dbrecord.altmax
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
        id: tripdata
        title: "Trip Data"
        items: tripitems
        x:      0
        y:      50
        width:  root.width
        height: 210

        DynamicItemModel {
            id: tripitems
            name: "tripitems"

            OptionInputItem { id: tripname;     text: "Name:     "; value: record.name;     onValueChanged: settings.setProperty("trip_defaultname",value) }
            OptionInputItem { id: triptime;     text: "Time:     "; value: record.triptime; readOnly: true }
            OptionInputItem { id: tripdist;     text: "Distance: "; value: record.tripdist.toFixed(0); readOnly: true }
        }
    }

    OptionList {
        id: speeddata
        title: "Speed Data"
        items: speeditems
        x:      0
        y:      255
        width:  root.width
        height: 210

        DynamicItemModel {
            id: speeditems
            name: "speeditems"

            OptionInputItem { id: tripavgspeed; text: "Average:  "; value: record.speedavg.toFixed(1); readOnly: true }
            OptionInputItem { id: tripminspeed; text: "Minimum:  "; value: record.speedmin.toFixed(1); readOnly: true }
            OptionInputItem { id: tripmaxspeed; text: "Maximum:  "; value: record.speedmax.toFixed(1); readOnly: true }
        }
    }

    OptionList {
        id: altdata
        title: "Altitude Data"
        items: altitems
        x:      0
        y:      450
        width:  root.width
        height: 320

        DynamicItemModel {
            id: altitems
            name: "altitems"

            OptionInputItem { id: tripascent;   text: "Ascent:   "; value: record.ascent.toFixed(0);   readOnly: true }
            OptionInputItem { id: tripdescent;  text: "Descent:  "; value: record.descent.toFixed(0);  readOnly: true }
            OptionInputItem { id: tripavgalt;   text: "Average:  "; value: record.altavg.toFixed(1);   readOnly: true }
            OptionInputItem { id: tripminalt;   text: "Minimum:  "; value: record.altmin.toFixed(1);   readOnly: true }
            OptionInputItem { id: tripmaxalt;   text: "Maximum:  "; value: record.altmax.toFixed(1);   readOnly: true }
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
        onRightClicked: { if (trackstatus.status == "idle") root.trkAdd(); else root.trkStop() }
        onLeftClicked:  { y = root.height; }
        //rightbuttonsrc: trackstatus.status=="idle"? "../Images/add-plain.png" : "../Images/stop-plain.png"
        rightbuttonsrc: "../Images/add-plain.png"

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

    TripSelectionPage  { id: tripSelectPage; onTripSelected:  root.tripid = tripid; }
    WaypointEditPage   { id: wptedit;        onWaypointSaved: wptSaved(wptid);      }
    TrackRecordingPage { id: trkedit;        onTrackSaved:    trkSaved(trkid);      }

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

    function wptSaved(id) {
        console.log("adding reference from trip: ", tripid, "to wpt: ", id)
        dbrecord.addWaypointReference(id)
        refreshData()
    }

    function rteShowList() {
        rtepage.visible = true;
        rtepage.y = 0
    }

    function rteAdd() {
    }

    function rteSelected(id) {
    }

    function rteSaved(id) {
    }

    function trkShowList() {
        trkpage.visible = true;
        trkpage.y = 0
    }

    function trkStop() {
        trkedit.stopTrack(-1)
    }

    function trkAdd() {
        trkedit.trkid = -1
        pageStack.push(trkedit)
    }

    function trkSelected(id) {
        trkedit.trkid = id
        pageStack.push(trkedit)
    }

    function trkSaved(id) {
        console.log("adding reference from trip: ", tripid, "to trk: ", id)
        dbrecord.addTrackReference(id)
        refreshData()
    }

    function trkStopped(id) {
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
