import QtQuick 1.0
import "../components"

OptionPage {
    id: root
    title: index == -1? "New Route Point" : "Route Point Edit"
    options: rteptoptions
    confirmbutton: true
    property int index: -1
    property int routeid: -1
    property MapView mapview: null

    property alias latitude:  rtelat.value
    property alias longitude: rtelon.value
    property alias altitude:  rtealt.value
    property alias name:      rtename.value

    signal routepointSaved(int index, string name, real lat, real lon, real alt)

    function saveRoutepoint(index,name,lat,lon,alt) {
        console.log("RoutepointEditPage.saveRoutepoint",index,name,lat,lon,alt)
        routepointSaved(index,name,lat,lon,alt)
    }

    Settings {
        id: settings
    }

    VisualItemModel {
        id: rteptoptions

        OptionList {
            id: rteptbox
            //title: "New Route Point"
            items: rteptitems

            DynamicItemModel {
                id: rteptitems
                name: "rteptitems"

                OptionInputItem { id: rtename;     text: "Name:";      onValueChanged: settings.setProperty("rtept_defaultname",value) }
                OptionInputItem { id: rtelat;      text: "Latitude:";  value: mapview.maplat }
                OptionInputItem { id: rtelon;      text: "Longitude:"; value: mapview.maplon }
                OptionInputItem { id: rtealt;      text: "Altitude:";  value: "0.0"          }

                Component.onCompleted: {
                    rtename.value = settings.getProperty("rtept_defaultname","rtept-000")
                }
            }
        }
    }
    onConfirm: {
        console.log("RoutePointEditPage.onConfirm")
        saveRoutepoint(root.index,rtename.value,rtelat.value,rtelon.value,rtealt.value)
        pageStack.pop()
    }
}
