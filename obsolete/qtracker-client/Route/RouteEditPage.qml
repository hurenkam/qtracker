import QtQuick 1.0
import "../Components"
import "../Map"

OptionPage {
    id: root
    title: index == -1? "New Route" : "Route Edit"
    options: rteoptions
    confirmbutton: true
    property int index: -1
    property int routeid: -1
    property MapView mapview: null
    property alias name: rtename.value
    property real rtetop
    property real rteleft
    property real rtebottom
    property real rteright

    signal routeSaved(int index, string name, real top, real left, real bottom, real right)

    function saveRoute(index,name,lat,lon,alt) {
        console.log("RouteEditPage.saveRoute",index,name,top,left,bottom,right)
        routeSaved(index,name,top,left,bottom,right)
    }

    RoutePointEditPage {
        id: rteptedit;
        routeid: root.routeid;
        mapview: root.mapview;
        onRoutepointSaved: rtepointbox.saveRoutepoint(index,name,lat,lon,alt)
    }

    VisualItemModel {
        id: rteoptions

        OptionList {
            id: rtebox
            title: "Route Options"
            items: rteitems

            DynamicItemModel {
                id: rteitems
                name: "rteitems"

                OptionInputItem { id: rtename; text: "Name:";   value: "rte" }
            }
        }

        RoutePointList {
            id: rtepointbox
            routeid: root.routeid
            title: "Route Points"
            onRoutepointSelected: {
                rteptedit.index = index;
                if (index >= 0) {
                    rteptedit.name = name;
                }
                pageStack.push(rteptedit);
            }
        }
    }
    onConfirm: {
        console.log("RouteEditPage.onConfirm")
        saveRoute(root.index,rtename.value,0,0,0,0)
        pageStack.pop()
    }
}
