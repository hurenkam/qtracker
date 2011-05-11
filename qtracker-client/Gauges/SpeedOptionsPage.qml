import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Speed Options"
    options: speedoptions
    confirmbutton: true
    signal optionsChanged()
    onConfirm: {
        console.log("SpeedOptionsPage.onConfirm")
        optionsChanged()
        pageStack.pop()
    }

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
