import QtQuick 1.0
import "../Components"

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
