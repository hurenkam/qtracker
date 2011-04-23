import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: index == -1? "New Route" : "Route Edit"
    options: rteoptions
    property int index: -1

    RoutePointEditPage { id: rteptedit }

    VisualItemModel {
        id: rteoptions

        OptionList {
            id: rtebox
            title: "Route Options"
            items: rteitems

            DynamicItemModel {
                id: rteitems
                name: "rteitems"

                OptionInputItem { id: rtename; text: "Name:";   value: "Home" }
            }
        }

        OptionList {
            id: rtepointbox
            title: "Route Points"
            items: rtepoints

            DynamicItemModel {
                id: rtepoints
                name: "rtepoints"

                OptionTextItem { id: newpoint; text: "<new>"; button: true }
            }
            onClicked: { rteptedit.index = index -1; pageStack.push(rteptedit); }
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
