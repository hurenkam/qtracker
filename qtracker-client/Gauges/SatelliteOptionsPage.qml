import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Satellite Options"
    options: satoptions

    VisualItemModel {
        id: satoptions

        OptionList {
            id: satbox
            //title: "Analog"
            items: satitems

            DynamicItemModel {
                id: satitems
                name: "satitems"
            }
        }
    }
}
