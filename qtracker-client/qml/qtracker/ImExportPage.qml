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
                name: "securityitems"
                VisualItemModel {
                    OptionItem { text: "Passcode"; button: true; onClicked: root.pageStack.push(security) }
                    Component.onCompleted: {
                        for (var i=0; i<count; i++) {
                            securityitems.append(children[i])
                        }
                    }
                }
            }
        }

        OptionBox {
            id: box2
            title: "General"
            items: generalitems

            DynamicItemModel {
                id: generalitems
                name: "generalitems"
                VisualItemModel {
                    OptionItem { text: "Days End" }
                    OptionItem { text: "Moment Sorting" }
                    OptionItem { text: "Week Starts On" }
                    OptionItem { text: "Sounds" }
                    OptionItem { text: "Photos" }
                    Component.onCompleted: {
                        for (var i=0; i<count; i++) {
                            generalitems.append(children[i])
                        }
                    }
                }
            }
        }

        OptionList {
            id: box3
            title: "Other"
            height: 170
            items: otheritems

            DynamicItemModel {
                id: otheritems
                name: "otheritems"
                VisualItemModel {
                    OptionItem { text: "Option a" }
                    OptionItem { text: "Option b" }
                    OptionItem { text: "Option c" }
                    OptionItem { text: "Option d" }
                    OptionItem { text: "Option e" }
                    OptionItem { text: "Option f" }
                    OptionItem { text: "Option g" }
                    OptionItem { text: "Option h" }
                    Component.onCompleted: {
                        for (var i=0; i<count; i++) {
                            otheritems.append(children[i])
                        }
                    }
                }
            }

            onClicked: console.log("other.onclicked:",index,text)
        }

    }
}
