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

    function itemClicked(index) {
        var wpt = database.categories[0].waypoints[index]
        if (index >= 0) {
            wptedit.name = wpt.name;
            wptedit.latitude = wpt.latitude;
            wptedit.longitude = wpt.longitude;
            wptedit.altitude = wpt.altitude;
        } else {
            wptedit.latitude = maplat;
            wptedit.longitude = maplon;
            wptedit.altitude = mapalt;
        }
        pageStack.push(wptedit);
    }
// } COMPAT

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
            onClicked: root.itemClicked(index)
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

            OptionInputItem { id: catname; text: "Name:" }
        }
    }

    tabs: TabLayout {
        //anchors.fill: parent
        x:      10
        y:      120
        width:  root.width - 20
        height: root.height - 180
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
                }
            }
        }
    }

    Component.onCompleted: {
        var cat = database.categories[0]
        cat.selectWaypoints(0,15)
        wptlist.model = cat.waypoints
        cat.selectRoutes(0,15)
        rtelist.model = cat.routes
        cat.selectTracks(0,15)
        trklist.model = cat.tracks
    }
}
/*
MainOptionPage {
    id: root
    title: "Settings"
    options: settingsmodel
    property Item mapview

    OptionPage { id: security; title: "Security"}

    TDatabase {
        id: database
    }

    Component {
        id: delegate
        TextInput { x: 0; y: index * 30; text: modelData.name }
    }

    VisualItemModel {
        id: settingsmodel

        Repeater {
            id: repeater
            delegate: delegate
        }

        Component.onCompleted: {
            database.categories[0].selectWaypoints(0,18)
            repeater.model = database.categories[0].waypoints;
        }
    }

    TCategory {
        id: category
    }

    VisualItemModel {
        id: settingsmodel

        OptionList {
            id: box1
            title: "Category " + category.name
            items: categoryitems

            DynamicItemModel {
                id: categoryitems
                name: "categoryitems"
                OptionTextItem { text: category.waypoints.length.toString(); button: true; onClicked: root.pageStack.push(security) }
                OptionTextItem { text: category.waypoints[0].name; button: true; onClicked: root.pageStack.push(security) }
            }
        }

        OptionList {
            id: box2
            title: "General"
            items: generalitems

            DynamicItemModel {
                id: generalitems
                name: "generalitems"

                OptionTextItem { text: "Days End" }
                OptionTextItem { text: "Moment Sorting" }
                OptionTextItem { text: "Week Starts On" }
                OptionTextItem { text: "Sounds" }
                OptionTextItem { text: "Photos" }
            }
        }

        OptionList {
            id: box3
            title: "Other"
            height: 170
            items: otheritems

            DynamicItemModel {
                id: otheritems
                property string name: "otheritems"
                OptionTextItem { text: "Option a"; button: true }
                OptionTextItem { text: "Option b"; button: true }
                OptionTextItem { text: "Option c"; button: true }
                OptionTextItem { text: "Option d"; button: true }
                OptionTextItem { text: "Option e"; button: true }
                OptionTextItem { text: "Option f"; button: true }
                OptionTextItem { text: "Option g"; button: true }
                OptionTextItem { text: "Option h"; button: true }
            }

            onClicked: console.log("other.onclicked:",index,text)
        }
    }
*/
/*
    GpxFile {
        id: gpxfile
        fileName: "c:\\data\\qtracker\\test.gpx"

        onFileNameChanged: console.log("GpxFile.onFileNameChanged()"); //: ",fileName);

        onWayPoint:        waypoints.append(wpt);

        onRouteFound:      console.log("GpxFile.onRouteFound()     ");
        onRouteName:       console.log("GpxFile.onRouteName():     ", routeName);
        onRoutePoint:      console.log("GpxFile.onRoutePoint():    ", rtept.name, rtept.latitude, rtept.longitude);

        onTrackFound:      console.log("GpxFile.onTrackFound()     ");
        onTrackName:       console.log("GpxFile.onTrackName():     ", trackName);
        onTrackPoint:      console.log("GpxFile.onTrackPoint():    ", trkpt.latitude, trkpt.longitude);

        Waypoints {
            id: gpxwpt
        }

        Routes {
            id: gpxrte
        }

        RoutePoints {
            id: gpxrtept
        }

        Tracks {
            id: gpxtrk
        }

        TrackPoints {
            id: gpxtrkpt
        }

        function exportGpx() {
            var i,j;
            reset();
            openGpx();
            for (i=0; i<gpxwpt.count; i++) {
                writeWayPoint(gpxwpt.get(i));
            }
            for (i=0; i<gpxrte.count; i++) {
                openRoute(gpxrte.get(i));
                for (j=0; i<gpxrtept.count; j++) {
                    writeRoutePoint(gpxrtept.get(j));
                }
                closeRoute();
            }
            for (i=0; i<gpxtrk.count; i++) {
                openRoute(gpxtrk.get(i));
                for (j=0; i<gpxtrkpt.count; j++) {
                    writeRoutePoint(gpxtrkpt.get(j));
                }
                closeRoute();
            }
            closeGpx();
        }

        function importGpx() {
            reset();
            parseGpx();
        }
    }
}
*/

