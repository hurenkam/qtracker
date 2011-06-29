import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Available Maps"
    options: maplist
    //property MapView mapview: null
    property int catid: -1
    property int tripid: -1

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
    title: "Map List"
    options: maplist

    //signal mapSelected(int index, string baseName, string fileName)
    signal mapSelected(int mapid, string name)

    VisualItemModel {
        id: maplist

        MapList {
            id: lst
            //title: "List"
            //onMapSelected: { root.mapSelected(index,baseName,fileName) }
            onMapSelected: { root.mapSelected(mapid, name) }
        }
    }
}
*/
