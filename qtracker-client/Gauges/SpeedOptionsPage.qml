import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Speed Options"
    options: speedoptions

    VisualItemModel {
        id: speedoptions

        OptionList {
            id: speedbox
            //title: "Analog"
            items: speeditems

            DynamicItemModel {
                id: speeditems
                name: "speeditems"
            }
        }
    }
}
