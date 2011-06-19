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

OptionPage {
    id: root
    title: "Settings"
    options: settingsmodel
    property Item mapview

    OptionPage { id: security; title: "Security"}

    VisualItemModel {
        id: settingsmodel

        OptionList {
            id: box1
            title: "Security"
            items: securityitems

            DynamicItemModel {
                id: securityitems
                name: "securityitems"
                OptionTextItem { text: "Passcode"; button: true; onClicked: root.pageStack.push(security) }
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
