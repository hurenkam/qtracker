import QtQuick 1.0
import "../Components"
import "../Waypoint"

OptionPage {
    id: root
    title: index==-1? "New Calibration Point": "Edit Calibration Point"
    options: caloptions
    confirmbutton: true
    property int index: -1
    property MapView mapview: null

    signal calibrationPointSaved(int index, real lat, real lon, real x, real y)

    function saveCalibrationPoint(index,lat,lon,x,y) {
        console.log("RefpointEditPage.addCalibrationPoint",index,lat,lon,x,y)
        calibrationPointSaved(index,lat,lon,x,y)
    }

    OptionPage {
        id: wptselect
        title: "Waypoint List"
        options: wptoptions

        WaypointEditPage { id: wptedit }

        VisualItemModel {
            id: wptoptions

            WaypointList {
                id: lst
                //title: "Waypoint List"
                onWaypointSelected: {
                    if (index >= 0) {
                        refname.value = name
                        reflat.value = lat
                        reflon.value = lon
                    }
                    pageStack.pop()
                }
            }
        }
    }

    VisualItemModel {
        id: caloptions

        OptionList {
            id: editrefpoint
            title: "Edit"
            items: refedititems

            DynamicItemModel {
                id: refedititems
                name: "refedititems"
                OptionInputItem { id: refname;        text: "Name:";      value: "cal"          }
                OptionInputItem { id: reflat;         text: "Latitude:";  value: mapview.maplat }
                OptionInputItem { id: reflon;         text: "Longitude:"; value: mapview.maplon }
                OptionInputItem { id: refx;           text: "X:";         value: mapview.mapx   }
                OptionInputItem { id: refy;           text: "Y:";         value: mapview.mapy   }
            }
        }

        OptionList {
            id: fromwpt
            items: fromwptitems

            DynamicItemModel {
                id: fromwptitems
                name: "fromwptitems"
                OptionTextItem {
                    text: "From Waypoint";
                    button: true;
                    onClicked: pageStack.push(wptselect)
                }
            }
        }
    }
    onConfirm: {
        console.log("RefpointEditPage.onConfirm")
        saveCalibrationPoint(root.index,reflat.value,reflon.value,refx.value,refy.value)
        pageStack.pop()
    }
}
