import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../Components"
import "../Main"

OptionPage {
    id: root
    title: "Map"
    options: mapmodel
    property int mapid: -1
    property TMap dbrecord
    leftbuttonsrc: "../Images/left-plain.svg"
    rightbutton: true
    rightbuttonsrc: "../Images/visible-plain.svg"

    signal mapSelected(int mapid)

    TDatabase {
        id: database
    }

    Settings { id: settings }

    MapSelectionPage      { id: mapSelectPage;      onMapSelected:      root.mapSelected(mapid) }
    RefpointSelectionPage { id: refpointSelectPage; mapid: root.mapid }

    VisualItemModel {
        id: mapmodel

        OptionList {
            id: edit
            title: "Options"
            items: optionitems

            DynamicItemModel {
                id: optionitems
                name: "optionitems"
                OptionInputItem  { id: map;       text: "Map";          value:dbrecord.name; button: true; onClicked: pageStack.push(mapSelectPage); }
                OptionInputItem  { id: refpoints; text: "Calibration";  value:"";            button: true; onClicked: pageStack.push(refpointSelectPage); }
                OptionInputItem  { id: datum;     text: "Datum";        value:"Wgs84";       button: true; }
            }
        }
    }

    function refreshData() {
        console.log("MapEditPage.refreshData(",root.mapid,")")
        dbrecord = database.getMap(root.mapid)
        console.log("dbrecord: ",dbrecord.mapid, dbrecord.name)
    }

    onMapidChanged: refreshData()
    Component.onCompleted: refreshData()
}
