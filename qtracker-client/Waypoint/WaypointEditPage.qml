import QtQuick 1.0
import "../Components"
import "../Map"

OptionPage {
    id: root
    title: index == -1? "New Waypoint" : "Waypoint Edit"
    options: wptoptions
    confirmbutton: true
    property MapView mapview: null
    property int index: -1
    property alias latitude:  wptlat.value
    property alias longitude: wptlon.value
    property alias altitude:  wptalt.value
    property alias name:      wptname.value

    signal waypointSaved(int index, string name, real lat, real lon, real alt)

    function saveWaypoint(index,name,lat,lon,alt) {
        console.log("WaypointEditPage.saveWaypoint",index,name,lat,lon,alt)
        waypointSaved(index,name,lat,lon,alt)
    }

    CategorySelectionPage { id: catselpage }

    VisualItemModel {
        id: wptoptions

        OptionList {
            id: wptbox
            //title: "New Waypoint"
            items: wptitems

            DynamicItemModel {
                id: wptitems
                name: "wptitems"

                OptionInputItem {
                    id: category;
                    text: "Category:";
                    value: "Waypoints";
                    button: true;
                    onClicked: root.pageStack.push(catselpage)
                }
                OptionInputItem { id: wptname;        text: "Name:";      value: "wpt"          }
                OptionInputItem { id: wptlat;         text: "Latitude:";  value: mapview.maplat }
                OptionInputItem { id: wptlon;         text: "Longitude:"; value: mapview.maplon }
                OptionInputItem { id: wptalt;         text: "Altitude:";  value: "0.0"          }
                //OptionInputItem { id: notes;       text: "Notes:";     value: ""              }
            }
        }
    }
    onConfirm: {
        console.log("WaypointEditPage.onConfirm",wptname.value,wptlat.value,wptlon.value,wptalt.value)
        saveWaypoint(root.index,wptname.value,wptlat.value,wptlon.value,wptalt.value)
        pageStack.pop()
    }
}
