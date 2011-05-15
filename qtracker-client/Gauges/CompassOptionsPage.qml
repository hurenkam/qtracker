import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Compass Options"
    options: compassoptions
    confirmbutton: true
    signal optionsChanged()
    property alias sourceindex:      sourceitems.ticked
    property alias orientationindex: orientationitems.ticked

    onConfirm: {
        console.log("CompassOptionsPage.onConfirm")
        optionsChanged()
        pageStack.pop()
    }

    VisualItemModel {
        id: compassoptions

        OptionList {
            id: sourcebox
            title: "Source"
            items: sourceitems

            RadioBox {
                id: sourceitems
                name: "compass_source"

                OptionRadioButton { text: "Magnetic North Sensor"; }
                OptionRadioButton { text: "GPS Heading";           }
            }
            onClicked: {
                console.log("sourceitems.onClicked",index)
                sourceitems.ticked = index
            }
        }

        OptionList {
            id: orientationbox
            title: "Orientation"
            items: orientationitems

            RadioBox {
                id: orientationitems
                name: "compass_orientation"

                OptionRadioButton { text: "Heading up";         }
                OptionRadioButton { text: "North up";           }
            }

            onClicked: {
                console.log("orientationitems.onClicked",index)
                orientationitems.ticked = index
            }
        }
    }
}
