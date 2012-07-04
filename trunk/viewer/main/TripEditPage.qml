import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../components"
import "../model"

OptionPage {
    id: root
    title: "Trip"
    imageSource: "../components/options-bg.png"
    rightbutton: true
    rightbuttonsrc: "options-plain.png"
    rightbuttonradius: 0
    leftbutton: true
    leftbuttonsrc: "left-plain.png"
    leftbuttonradius: 0
    property int tripid: live.tripid
    property TTrip dbrecord

    tools: ToolBarLayout {
        id: maintools
        ToolButton { id: okbutton;   source: "visible-plain.png";   bgcolor: "black"; bgradius: 0 }
        ToolButton { visible: false; showbg: false }
        ToolButton { visible: false; showbg: false }
        ToolButton { id: wptbutton;  source: "flag-plain.png";      bgcolor: "black"; bgradius: 0; onClicked: wptShowList() }
        ToolButton { id: rtebutton;  source: "route-plain.png";     bgcolor: "black"; bgradius: 0; onClicked: rteShowList() }
        ToolButton { id: trkbutton;  source: "hiker-plain.png";     bgcolor: "black"; bgradius: 0; onClicked: trkShowList() }
        height: 60
    }

    function updateTrip(key,name,state) {
        //console.log("TripEditPage.updateTrip()",key,name,state)
        live.name = name
        live.tripid = key
        live.tripstate = state
    }

    function updateTime(current,elapsed,monitor) {
        //console.log("TripEditPage.updateTime()",current,elapsed,monitor)
        live.triptime = elapsed
    }

    function updateAltitude(current,average,minimum,maximum,ascent,descent) {
        //console.log("TripEditPage.updateAltitude()",current,average,minimum,maximum,ascent,descent)
        live.altavg = average
        live.altmin = minimum
        live.altmax = maximum
        live.ascent = ascent
        live.descent = descent
    }

    function updateSpeed(current,average,minimum,maximum) {
        //console.log("TripEditPage.updateSpeed()",current,average,minimum,maximum)
        live.speedavg = average
        live.speedmin = minimum
        live.speedmax = maximum
    }

    function updateDistance(current,monitor) {
        //console.log("TripEditPage.updateDistance()",current,monitor)
        live.tripdist = current
    }

    XmlRpc {
        id: server
    }

    QtObject {
        id: live
        property string name: ""
        property int    tripid: -1
        property string tripstate: ""
        property date   triptime: "2000-01-01"
        property double tripdist: 0
        property double speedmin: 0
        property double speedmax: 0
        property double speedavg: 0
        property double altmin:   0
        property double altmax:   0
        property double altavg:   0
        property double ascent:   0
        property double descent:  0
    }

    QtObject {
        id: record
        property bool    current: ((live.tripid === root.tripid) || !dbrecord)
        property int     tripid:   (current)? live.tripid   : root.tripid
        property string  name:     (current)? live.name     : dbrecord.name
        property date    triptime: (current)? live.triptime : dbrecord.triptime
        property double  tripdist: (current)? live.tripdist : dbrecord.tripdist
        property double  speedavg: (current)? live.speedavg : dbrecord.speedavg
        property double  speedmin: (current)? live.speedmin : dbrecord.speedmin
        property double  speedmax: (current)? live.speedmax : dbrecord.speedmax
        property double  ascent:   (current)? live.ascent   : dbrecord.ascent
        property double  descent:  (current)? live.descent  : dbrecord.descent
        property double  altavg:   (current)? live.altavg   : dbrecord.altavg
        property double  altmin:   (current)? live.altmin   : dbrecord.altmin
        property double  altmax:   (current)? live.altmax   : dbrecord.altmax
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
            OptionInputItem { id: triptime;     text: "Time:     "; value: Qt.formatTime(record.triptime,"hh:mm:ss"); readOnly: true }
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
        onRightClicked: { if (server.trackstate == "idle") root.trkAdd(); else root.trkStop() }
        onLeftClicked:  { y = root.height; }
        rightbuttonsrc: server.trackstate=="idle"? "../Images/add-plain.png" : "../Images/stop-plain.png"
        //rightbuttonsrc: "../Images/add-plain.png"

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
