import QtQuick 1.0
import "../Components"
import "../Main"

/*
OptionPage {
    id: root
    title: "Available Maps"
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
*/

MainOptionPage {
    id: root
    title: "Map"
    options: mapmodel
    //property int currentid: mapview? mapview.mapid : -1
    //property string currentmap: mapview? mapview.mapname : "<unknown>"
    //property MapView mapview: undefined
/*
    onMapviewChanged: {
        currentmap = mapview.mapname
        currentid = mapview.mapid
    }
*/
    signal mapSelected(int mapid, string name)
    onMapidChanged:   console.log("MapEditPage.onMapidChanged(",mapid,")")
    onMapnameChanged: console.log("MapEditPage.onMapnameChanged(",mapname,")")

    Settings { id: settings }

    RefpointSelectionPage {
        id: calselectpage;
        //mapview: root.mapview
        mapid:   root.mapid
        mapname: root.mapname
        maplat:  root.maplat
        maplon:  root.maplon
        mapx:    root.mapx
        mapy:    root.mapy
    }

    Loader {
        id: pageloader
    }

    Component {
        id: pagesource

        MapSelectionPage {
            id: mapselectpage;
            onMapSelected: {
                //root.currentid = mapid
                //root.currentmap = name
                //root.mapname = name
                //root.mapid = mapid
                root.mapSelected(mapid,name)
                //pageStack.pop()
            }
        }
    }

    function showMapSelectionPage() {
        pageloader.sourceComponent = pagesource
        pageStack.push(pageloader.item)
    }

    function showRefpointSelectionPage() {
        pageStack.push(calselectpage)
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
                //OptionInputItem  { id: mapname;   text: "Map";          value:currentmap;   button: true; onClicked: showMapSelectionPage() } //pageStack.push(mapselectpage) }
                //OptionInputItem  { id: refpoints; text: "Calibration";  value:"";           button: true; onClicked: pageStack.push(calselectpage) }
                //OptionInputItem  { id: datum;     text: "Datum";        value:"Wgs84";      button: true; }
                OptionInputItem  { id: map;       text: "Map";          value:mapname;      button: true; onClicked: showMapSelectionPage() } //pageStack.push(mapselectpage) }
                OptionInputItem  { id: refpoints; text: "Calibration";  value:"";           button: true; onClicked: showRefpointSelectionPage() }
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
        if ((visible==true) && (mapname=="")) showMapSelectionPage(); //pageStack.push(mapselectpage);
    }
}
