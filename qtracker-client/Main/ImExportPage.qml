import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Settings"
    options: settingsmodel
    property Item mapview

    OptionPage { id: security; title: "Security"}

    VisualItemModel {
        id: settingsmodel

        OptionList {
            id: box1
            title: "Security"
            items: securityitems

            DynamicItemModel {
                id: securityitems
                name: "securityitems"
                OptionTextItem { text: "Passcode"; button: true; onClicked: root.pageStack.push(security) }
            }
        }

        OptionList {
            id: box2
            title: "General"
            items: generalitems

            DynamicItemModel {
                id: generalitems
                name: "generalitems"

                OptionTextItem { text: "Days End" }
                OptionTextItem { text: "Moment Sorting" }
                OptionTextItem { text: "Week Starts On" }
                OptionTextItem { text: "Sounds" }
                OptionTextItem { text: "Photos" }
            }
        }

        OptionList {
            id: box3
            title: "Other"
            height: 170
            items: otheritems

            DynamicItemModel {
                id: otheritems
                property string name: "otheritems"
                OptionTextItem { text: "Option a"; button: true }
                OptionTextItem { text: "Option b"; button: true }
                OptionTextItem { text: "Option c"; button: true }
                OptionTextItem { text: "Option d"; button: true }
                OptionTextItem { text: "Option e"; button: true }
                OptionTextItem { text: "Option f"; button: true }
                OptionTextItem { text: "Option g"; button: true }
                OptionTextItem { text: "Option h"; button: true }
            }

            onClicked: console.log("other.onclicked:",index,text)
        }

    }
}
