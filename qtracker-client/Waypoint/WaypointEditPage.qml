import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: index == -1? "New Waypoint" : "Waypoint Edit"
    options: wptoptions
    property int index: -1

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
                OptionInputItem { id: name;        text: "Name:";      value: "Home"        }
                OptionInputItem { id: lat;         text: "Latitude:";  value: "53.128"      }
                OptionInputItem { id: lon;         text: "Longitude:"; value: "5.2801"      }
                OptionInputItem { id: alt;         text: "Altitude:";  value: "29.8"        }
                //OptionInputItem { id: notes;       text: "Notes:";     value: ""            }
            }
        }

        OptionList {
            id: confirm
            items: confirmitems

            DynamicItemModel {
                id: confirmitems
                name: "confirmitems"
                OptionTextItem {
                    text: "Confirm";
                    button: true;
                    buttonsource: "visible.svg";
                    onClicked: pageStack.pop()
                }
            }
        }
    }
}
