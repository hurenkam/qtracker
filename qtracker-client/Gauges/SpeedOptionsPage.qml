import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Speed Options"
    options: speedoptions
    confirmbutton: true
    signal optionsChanged()
    property alias analogindex: analogitems.ticked
    property alias topindex:    analogitems.ticked
    property alias bottomindex: analogitems.ticked

    Settings { id: settings }

    VisualItemModel {
        id: speedoptions

        OptionList {
            id: analogbox
            title: "Analog"
            items: analogitems

            RadioBox {
                id: analogitems
                name: "speed_analog"

                OptionRadioButton { text: "Current Speed";      }
                OptionRadioButton { text: "Minimum Speed";      }
                OptionRadioButton { text: "Maximum Speed";      }
                OptionRadioButton { text: "Average Speed";      }
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
                name: "speed_top"

                OptionRadioButton { text: "Current Speed";      }
                OptionRadioButton { text: "Minimum Speed";      }
                OptionRadioButton { text: "Maximum Speed";      }
                OptionRadioButton { text: "Average Speed";      }
                OptionRadioButton { text: "Trip Distance";      }
                OptionRadioButton { text: "Monitor Distance";   }
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
                name: "speed_bottom"

                OptionRadioButton { text: "Current Speed";      }
                OptionRadioButton { text: "Minimum Speed";      }
                OptionRadioButton { text: "Maximum Speed";      }
                OptionRadioButton { text: "Average Speed";      }
                OptionRadioButton { text: "Trip Distance";      }
                OptionRadioButton { text: "Monitor Distance";   }
            }

            onClicked: {
                console.log("bottomitems.onClicked",index)
                bottomitems.ticked = index
            }
        }
    }
    onConfirm: {
        console.log("SpeedOptionsPage.onConfirm")
        optionsChanged()
        pageStack.pop()
    }
}
