import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../Components"

Database {
    id: database
    table: "maps"
    property variant location: []

    function updateFilter() {
        if (!location.length) return;

        var filter = "";
        if (location.length == 2) {
            filter +=           "west<="  + root.location[1].toString();
            filter += " AND " + "east>="  + root.location[1].toString();
            filter += " AND " + "north>=" + root.location[0].toString();
            filter += " AND " + "south<=" + root.location[0].toString();
        }
        root.filter=filter;
        console.log("Maps.updateFilter(): ", filter)
        for (var i=0; i<count; i++) {
            console.log("Map: ",get(i).mapid,get(i).name)
        }
    }

    function setLocation(latitude,longitude) {
        root.location = [ latitude,longitude ];
    }

    function clearLocation() {
        root.location = []
    }
/*
    property QtObject wrapper: GpxFile {
        id: gpxfile
        property int mapid: -1

        onFileNameChanged: {
            console.log("GpxFile.onFileNameChanged(): ",fileName)
            reset()
            parseGpx()
        }

        onRefPoint:        console.log("GpxFile.onRefpointFound()",refpt.x,refpt.y,refpt.latitude,refpt.longitude);
        onResolution:      console.log("GpxFile.onResolutionFound()",resolution.width,resolution.height);
    }

    function importMap(name,folder) {
        console.log("Maps.importMap(",name,",",folder,")")

        gpxfile.mapid = database.append({ name: name} )
        gpxfile.fileName = folder+name+".xml"
        //gpxfile.fileName = "c:\\data\\qtracker\\maps\\51a_oisterwijk.xml"
    }
*/
    onLocationChanged: updateFilter()
    Component.onCompleted: {
        console.log("Maps.onCompleted")
        database.exec("CREATE TABLE IF NOT EXISTS maps (mapid INTEGER PRIMARY KEY, name TEXT, filename TEXT, north NUMBER, east NUMBER, south NUMBER, west NUMBER);");
    }
}
