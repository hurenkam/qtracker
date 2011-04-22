import QtQuick 1.0

OptionPage {
    id: root
    title: "Waypoint"
    options: wptoptions

    CategoryMenu { id: categorymenu }

    VisualItemModel {
        id: wptoptions

        OptionList {
            id: wptbox
            title: "New Waypoint"
            items: wptitems

            DynamicItemModel {
                id: wptitems
                name: "wptitems"

                OptionInputItem {
                    id: category;
                    text: "Category:";
                    value: "Waypoints";
                    button: true;
                    onClicked: root.pageStack.push(categorymenu)
                }
                OptionInputItem { id: name;        text: "Name:";      value: "Home"        }
                OptionInputItem { id: lat;         text: "Latitude:";  value: "53.128"      }
                OptionInputItem { id: lon;         text: "Longitude:"; value: "5.2801"      }
                OptionInputItem { id: alt;         text: "Altitude:";  value: "29.8"        }
                //OptionInputItem { id: notes;       text: "Notes:";     value: ""            }
            }
        }
    }
}
