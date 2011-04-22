import QtQuick 1.0

OptionPage {
    id: root
    title: "Calibration Point"
    options: caloptions

    CategoryMenu { id: categorymenu }

    VisualItemModel {
        id: caloptions

        OptionList {
            id: editrefpoint
            title: "Edit"
            items: refedititems

            DynamicItemModel {
                id: refedititems
                name: "refedititems"
                OptionInputItem { id: refname;        text: "Name:";      value: "Home"        }
                OptionInputItem { id: reflat;         text: "Latitude:";  value: "53.128"      }
                OptionInputItem { id: reflon;         text: "Longitude:"; value: "5.2801"      }
                OptionInputItem { id: refx;           text: "X:";         value: "0"           }
                OptionInputItem { id: refy;           text: "Y:";         value: "0"           }
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
                    //onClicked: pageStack.push(wptselect)
                }
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
                    buttonsource: "qrc:/images/visible.svg";
                    onClicked: pageStack.pop()
                }
            }
        }
    }
}
