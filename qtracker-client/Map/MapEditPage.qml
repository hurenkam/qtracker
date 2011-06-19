import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Map List"
    options: maplist
    property MapView mapview: null

    signal mapSelected(int mapid, string name)

    Component {
        id: mapimportsrc;
        MapImportPage
        {
            id: importPage
            onFileSelected: lst.addMap(text,folder)
        }
    }

    Loader {
        id: pageloader
    }

    VisualItemModel {
        id: maplist

        MapList {
            id: lst
            onMapSelected: {
                root.mapSelected(mapid,name)
                pageStack.pop()
            }
            onImportMap: {
                console.log("MapEditPage.importMap()")
                pageloader.sourceComponent = mapimportsrc
                pageStack.push(pageloader.item)
            }
        }
    }
}

/*
OptionPage {
    id: root
    title: "Map"
    options: mapmodel
    property int currentid: -1
    property string currentmap: "<unknown>"
    property MapView mapview: null
    //onMapviewChanged: currentmap = mapview.mapname

    signal mapSelected(int mapid, string name)

    Settings { id: settings }

    RefpointSelectionPage { id: calselectpage; mapview: root.mapview }

    Loader {
        id: pageloader
    }

    Component {
        id: pagesource

        MapSelectionPage {
            id: mapselectpage;
            onMapSelected: {
                root.currentid = mapid
                root.currentmap = name
                mapname.value = name
                root.mapSelected(mapid,name)
                pageStack.pop()
            }
        }
    }

    function showMapSelectionPage() {
        pageloader.sourceComponent = pagesource
        pageStack.push(pageloader.item)
    }

    VisualItemModel {
        id: mapmodel

        OptionList {
            id: edit
            title: "Options"
            items: optionitems

            DynamicItemModel {
                id: optionitems
                name: "optionitems"
                OptionInputItem  { id: mapname;   text: "Map";          value:currentmap;   button: true; onClicked: showMapSelectionPage() } //pageStack.push(mapselectpage) }
                OptionInputItem  { id: refpoints; text: "Calibration";  value:"";           button: true; onClicked: pageStack.push(calselectpage) }
                OptionInputItem  { id: datum;     text: "Datum";        value:"Wgs84";      button: true; }
            }
//
            function base(filename) {
                var txt = String(filename);
                var p1 = txt.lastIndexOf('/');
                var p2 = txt.lastIndexOf('.');
                return txt.slice(p1+1,p2);
            }
//
        }
    }
    onVisibleChanged: {
        if ((visible==true) && (currentmap=="")) showMapSelectionPage(); //pageStack.push(mapselectpage);
    }
}
*/
