import QtQuick 1.0

OptionPage {
    id: root
    title: "Map"
    options: mapmodel

    signal mapSelected(int index, string baseName, string fileName)

    CalibrationMenu  { id: calselectpage }

    MapSelectionPage {
        id: mapselectpage;
        onMapSelected: {
            mapname.value = baseName
            root.mapSelected(index,baseName,fileName)
            //pageStack.pop()
        }
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
                OptionInputItem  { id: mapname; text: "Map";   value:"no map"; button: true; onClicked: pageStack.push(mapselectpage) }
                OptionInputItem  { id: datum;   text: "Datum"; value:"Wgs84";  button: true; }
                //OptionTextItem   { text: "Select Calibration Point"; button: true; onClicked: pageStack.push(calselectpage) }
            }
        }

        OptionList {
            id: editrefpoint
            title: "Calibration Points"
            items: refpoints

            DynamicItemModel {
                id: refpoints
                name: "securityitems"
                OptionTextItem  { id: newrefpoint; text: "[Add]"; button: true; onClicked: pageStack.push(calselectpage) }
/*
                OptionInputItem { id: refname;        text: "Name:";      value: "Home"        }
                OptionInputItem { id: reflat;         text: "Latitude:";  value: "53.128"      }
                OptionInputItem { id: reflon;         text: "Longitude:"; value: "5.2801"      }
                OptionInputItem { id: refx;           text: "X:";         value: "0"           }
                OptionInputItem { id: refy;           text: "Y:";         value: "0"           }
*/
            }
        }
    }
}
