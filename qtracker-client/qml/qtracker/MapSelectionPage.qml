import QtQuick 1.0

OptionPage {
    id: root
    title: "Map Selection"
    options: maplist

    signal mapSelected(int index, string baseName, string fileName)

    VisualItemModel {
        id: maplist

        MapList {
            id: lst
            x: 0; width: parent.width;
            anchors.top: edit.bottom;
            anchors.bottom: parent.bottom;
            title: "List"
            onMapSelected: { root.mapSelected(index,baseName,fileName); root.cancel(); mapname.value = baseName }
        }
    }
}
