import QtQuick 1.0

OptionPage {
    id: root
    title: "Settings"
    options: settingsmodel

    OptionPage { id: security; title: "Security"}

    VisualItemModel {
        id: settingsmodel

        OptionBox {
            id: box1
            title: "Security"
            items: securityitems

            DynamicItemModel {
                id: securityitems
                OptionItem { text: "Passcode"; button: true; onClicked: root.pageStack.push(security) }
            }
        }

        OptionBox {
            id: box2
            title: "General"
            items: generalitems

            DynamicItemModel {
                id: generalitems
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
                OptionItem { text: "Option a" }
                OptionItem { text: "Option b" }
                OptionItem { text: "Option c" }
                OptionItem { text: "Option d" }
                OptionItem { text: "Option e" }
                OptionItem { text: "Option f" }
                OptionItem { text: "Option g" }
                OptionItem { text: "Option h" }
            }

            onClicked: console.log("other.onclicked:",index,text)
        }

    }
}
