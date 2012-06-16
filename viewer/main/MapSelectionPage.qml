import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../components"
import "../main"

OptionPage {
    id: root
    title: "Map List"
    leftbuttonsrc: "left-plain.png"
    rightbutton: true
    rightbuttonsrc: "add-plain.png"

    signal mapSelected(int mapid)

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
            index2: index
            onClicked: ListView.view.itemClicked(index)
        }
    }

    Rectangle {
        id: mapbox
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
            id: maplist
            delegate: delegate

            function itemClicked(index) {
                pageStack.pop()
                root.mapSelected(model[index].mapid)
            }
        }
    }

    function refreshData() {
        console.log("MapSelectionPage.refreshData()")
        database.refresh()
        maplist.model = database.maps
    }

    TMap {
        id: importmap
        onMapidChanged: {
            console.log("MapSelectionPage.importmap.onMapidChanged(",mapid,")")
        }
    }

    TRefpoint {
        id: importref
    }
/*
    GpxFile {
        id: importfile
        onRefPoint: {
            console.log("MapSelectionPage.importfile.onRefPoint(",refpt,")")
            importref.refid = -1
            importref.mapid = importmap.mapid
            importref.latitude  = refpt["latitude"]
            importref.longitude = refpt["longitude"]
            importref.x = refpt["x"]
            importref.y = refpt["y"]
            importref.save()
        }
        onResolution: {
            console.log("MapSelectionPage.importfile.onResolution(",resolution,")")
        }
    }
*/
    MapImportPage {
        id: importpage
        onFileSelected: {
            console.log("MapSelectionPage.importpage.onFileSelected(",text,",",filename,")")
            importmap.mapid = -1
            importmap.name = text
            importmap.filename = filename
            importmap.save();
            importfile.fileName = folder + text + ".xml"
            importfile.parseGpx();
            pageStack.pop();
            root.refreshData();
        }
    }

    onRightClicked: pageStack.push(importpage)
    Component.onCompleted: refreshData()
}
