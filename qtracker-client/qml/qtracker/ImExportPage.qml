import QtQuick 1.0

OptionPage {
    id: root
    title: "Settings"
    options: settingsmodel

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
                OptionItem { text: "Passcode"; button: true; onClicked: root.pageStack.push(security) }
            }
        }

        OptionList {
            id: box2
            title: "General"
            items: generalitems

            DynamicItemModel {
                id: generalitems
                name: "generalitems"

                OptionItem { text: "Days End" }
                OptionItem { text: "Moment Sorting" }
                OptionItem { text: "Week Starts On" }
                OptionItem { text: "Sounds" }
                OptionItem { text: "Photos" }
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
                OptionItem { text: "Option a"; button: true }
                OptionItem { text: "Option b"; button: true }
                OptionItem { text: "Option c"; button: true }
                OptionItem { text: "Option d"; button: true }
                OptionItem { text: "Option e"; button: true }
                OptionItem { text: "Option f"; button: true }
                OptionItem { text: "Option g"; button: true }
                OptionItem { text: "Option h"; button: true }
            }

            onClicked: console.log("other.onclicked:",index,text)
        }

    }
}
