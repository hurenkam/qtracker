import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import QmlTrackerExtensions 1.0
import "../Components"
import "../Main"
import "../Waypoint"
import "../Route"
import "../Track"

TabOptionPage {
    id: root
    title: "Trip"
    imageSource: "../Components/options-bg.png"
    rightbutton: true
    rightbuttonsrc: "../Main/options.svg"
    property int tripid: 1
    property TTrip dbrecord

// COMPAT {
    property int    mapid:   -1
    property string mapname: "<no map>"
    property double maplat:  0.0
    property double maplon:  0.0
    property double mapalt:  0.0
    property int    mapx:    0
    property int    mapy:    0

    function setupMapArguments(id, name, lat, lon, alt, x, y) {
        console.log("MainOptionPage.setupMapArguments(",id,name,lat,lon,alt,x,y,")")
        mapid = id
        mapname = name
        maplat = lat
        maplon = lon
        mapalt = alt
        mapx = x
        mapy = y
    }

    function wptSelected(id) {
        var wpt = database.getWaypoint(id)
        //if (index >= 0) {
            wptedit.name = wpt.name;
            wptedit.latitude = wpt.latitude;
            wptedit.longitude = wpt.longitude;
            wptedit.altitude = wpt.altitude;
        //} else {
        //    wptedit.latitude = maplat;
        //    wptedit.longitude = maplon;
        //    wptedit.altitude = mapalt;
        //}
        pageStack.push(wptedit);
    }

    function rteSelected(id) {
        var rte = database.getRoute(id)
    }

    function trkSelected(id) {
        var trk = database.getTrack(id)
    }

// } COMPAT

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

    WaypointEditPage {
        id: wptedit;
        //mapview: root.mapview;
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
        id: tripdata
        //title: "Trip Data"
        items: tripitems
        x:      0
        y:      50
        width:  root.width
        height: root.height - 100

        DynamicItemModel {
            id: tripitems
            name: "tripitems"

            OptionInputItem { id: tripname;     text: "Name:        "; value: record.name;     onValueChanged: settings.setProperty("trip_defaultname",value) }
            OptionInputItem { id: triptime;     text: "Time:        "; value: record.triptime; readOnly: true }
            OptionInputItem { id: tripdist;     text: "Distance:    "; value: record.tripdist; readOnly: true }

            OptionInputItem { id: tripavgspeed; text: "Speed Avg:   "; value: record.speedavg; readOnly: true }
            OptionInputItem { id: tripminspeed; text: "Speed Min:   "; value: record.speedmin; readOnly: true }
            OptionInputItem { id: tripmaxspeed; text: "Speed Max:   "; value: record.speedmax; readOnly: true }

            OptionInputItem { id: tripascent;   text: "Ascent:      "; value: record.ascent;   readOnly: true }
            OptionInputItem { id: tripdescent;  text: "Descent:     "; value: record.descent;  readOnly: true }
            OptionInputItem { id: tripavgalt;   text: "Altitude Avg:"; value: record.altavg;   readOnly: true }
            OptionInputItem { id: tripminalt;   text: "Altitude Min:"; value: record.altmin;   readOnly: true }
            OptionInputItem { id: tripmaxalt;   text: "Altitude Max:"; value: record.altmax;   readOnly: true }
        }
    }

    tabs: TabLayout {
        //anchors.fill: parent
        x:      10
        y:      60
        width:  root.width - 20
        height: root.height - 120
        id: tablayout

        TabItem {
            title: "Waypoints"
            Rectangle {
                id: wptbox
                anchors.margins: 10
                x: 0
                y: 0
                width: parent.width
                height: root.height - 200
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
                height: root.height - 200
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
                height: root.height - 200
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
        console.log("TripEditPage.refreshData(",root.tripid,")")
        dbrecord = database.getTrip(root.tripid)
        console.log("dbrecord: ",dbrecord.tripid, dbrecord.name)

        dbrecord.selectWaypoints(0,15)
        wptlist.model = dbrecord.waypoints
        dbrecord.selectRoutes(0,15)
        rtelist.model = dbrecord.routes
        dbrecord.selectTracks(0,15)
        trklist.model = dbrecord.tracks
    }

    TripSelectionPage { id: tripSelectPage; onTripSelected: root.tripid = tripid; }
    function selectTrip() {
        pageStack.push(tripSelectPage)
    }

    onRightClicked: selectTrip()
    onTripidChanged: refreshData()
    Component.onCompleted: refreshData()
}
