import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Altitude Options"
    options: altoptions

    VisualItemModel {
        id: altoptions

        OptionList {
            id: altbox
            //title: "Analog"
            items: altitems

            DynamicItemModel {
                id: altitems
                name: "analogitems"
            }
        }
    }
}
