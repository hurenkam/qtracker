import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: index == -1? "New Route Point" : "Route Point Edit"
    options: rteptoptions
    property int index: -1

    VisualItemModel {
        id: rteptoptions

        OptionList {
            id: rteptbox
            //title: "New Route Point"
            items: rteptitems

            DynamicItemModel {
                id: rteptitems
                name: "rteptitems"

                OptionInputItem { id: name;        text: "Name:";      value: "Home"        }
                OptionInputItem { id: lat;         text: "Latitude:";  value: "53.128"      }
                OptionInputItem { id: lon;         text: "Longitude:"; value: "5.2801"      }
                OptionInputItem { id: alt;         text: "Altitude:";  value: "29.8"        }
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
