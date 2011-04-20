import QtQuick 1.0

OptionPage {
    id: root
    title: "Waypoint"
    options: wptoptions

    VisualItemModel {
        id: wptoptions
        OptionBox {
            id: wptbox
            title: "New Waypoint"
            items: wptitems

            DynamicItemModel {
                id: wptitems
                name: "wptitems"

                VisualItemModel {
                    OptionItem { id: category;    text: "<category>" }
                    OptionItem { id: name;        text: "Name:"      }
                    OptionItem { id: lat;         text: "Latitude:"  }
                    OptionItem { id: lon;         text: "Longitude:" }
                    OptionItem { id: alt;         text: "Altitude:"  }
                    OptionItem { id: notes;       text: "Notes:"     }
                    Component.onCompleted: {
                        for (var i=0; i<count; i++) {
                            wptitems.append(children[i])
                        }
                    }
                }
            }
        }
    }
}

/*
+-----------------------+
| <Catagory>          > |
+-----------------------+
| Name:      __________ |
+-----------------------+
| Latitude:  __________ |
+-----------------------+
| Longitude: __________ |
+-----------------------+
| Altitude:  __________ |
+-----------------------+
| Notes:     __________ |
+-----------------------+
*/

/*
Page {
    id: root

    function confirm() {
        pageStack.pop();
    }
    function cancel() {
        pageStack.pop();
    }
/ *
    WaypointEdit {
        id: edit
    }
* /
    function newentry() {
        //pageStack.push(edit)
    }

    MenuEntry {
        id: heading
        x: 2; y: 2
        width: parent.width
        height: 54
        text: "Waypoint Menu"
        leftButtonVisible: true
        onLeftClicked: root.cancel();
    }

    MenuEntry {
        id: start
        anchors.top: heading.bottom
        width: parent.width
        height: 54
        text: "New Waypoint"
        rightButtonVisible: true
        onRightClicked: root.newentry();
    }

    MenuEntry {
        id: list
        anchors.top: start.bottom
        anchors.bottom: parent.bottom
        width: parent.width
/ *
        WaypointList {
            id: listbox
            anchors.fill: parent
        }
* /
    }
}
*/

/*
New
=================
Name:
Position:
Altitude:
Time:
| save | cancel |

List
=================
d e s <name>
| cancel |
*/
