import QtQuick 1.0

OptionPage {
    id: root
    title: "Waypoint"
    options: wptoptions

    VisualItemModel {
        id: wptoptions

        OptionList {
            id: wptbox
            title: "New Waypoint"
            items: wptitems

            DynamicItemModel {
                id: wptitems
                name: "wptitems"

                OptionItem { id: category;    text: "<category>" }
                OptionItem { id: name;        text: "Name:"      }
                OptionItem { id: lat;         text: "Latitude:"  }
                OptionItem { id: lon;         text: "Longitude:" }
                OptionItem { id: alt;         text: "Altitude:"  }
                OptionItem { id: notes;       text: "Notes:"     }
            }
        }
    }
}
