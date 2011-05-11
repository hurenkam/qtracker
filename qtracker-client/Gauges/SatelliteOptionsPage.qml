import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Satellite Options"
    options: satoptions
    confirmbutton: true
    signal optionsChanged()
    onConfirm: {
        console.log("ClockOptionsPage.onConfirm")
        optionsChanged()
        pageStack.pop()
    }

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
