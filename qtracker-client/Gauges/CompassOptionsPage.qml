import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Compass Options"
    options: compassoptions

    VisualItemModel {
        id: compassoptions

        OptionList {
            id: compassbox
            //title: "Analog"
            items: compassitems

            DynamicItemModel {
                id: compassitems
                name: "compassitems"
            }
        }
    }
}
