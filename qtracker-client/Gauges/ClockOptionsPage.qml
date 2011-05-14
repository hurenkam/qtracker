import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Clock Options"
    options: clockoptions
    confirmbutton: true
    signal optionsChanged()
    property alias analogindex: analogitems.ticked
    property alias topindex:    analogitems.ticked
    property alias bottomindex: analogitems.ticked

    Settings { id: settings }

    VisualItemModel {
        id: clockoptions

        OptionList {
            id: analogbox
            title: "Analog"
            items: analogitems

            RadioBox {
                id: analogitems
                name: "clock_analog"

                OptionRadioButton { text: "Current Time";      }
                OptionRadioButton { text: "Elapsed Time";      }
                OptionRadioButton { text: "Monitor Time";      }
            }
            onClicked: {
                console.log("analogitems.onClicked",index)
                analogitems.ticked = index
            }
        }
        OptionList {
            id: topbox
            title: "Top"
            items: topitems

            RadioBox {
                id: topitems
                name: "clock_top"

                OptionRadioButton { text: "Current Time";      }
                OptionRadioButton { text: "Elapsed Time";      }
                OptionRadioButton { text: "Monitor Time";      }
            }

            onClicked: {
                console.log("topitems.onClicked",index)
                topitems.ticked = index
            }
        }
        OptionList {
            id: bottombox
            title: "Bottom"
            items: bottomitems

            RadioBox {
                id: bottomitems
                name: "clock_bottom"

                OptionRadioButton { text: "Current Time";      }
                OptionRadioButton { text: "Elapsed Time";      }
                OptionRadioButton { text: "Monitor Time";      }
            }

            onClicked: {
                console.log("bottomitems.onClicked",index)
                bottomitems.ticked = index
            }
        }
    }
    onConfirm: {
        console.log("ClockOptionsPage.onConfirm")
        optionsChanged()
        pageStack.pop()
    }
}
