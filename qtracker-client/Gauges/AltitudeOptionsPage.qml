import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Altimeter Options"
    options: altoptions
    confirmbutton: true
    signal optionsChanged()
    property alias analogindex: analogitems.ticked
    property alias topindex:    topitems.ticked
    property alias bottomindex: bottomitems.ticked

    Settings { id: settings }

    VisualItemModel {
        id: altoptions

        OptionList {
            id: analogbox
            title: "Analog"
            items: analogitems

            RadioBox {
                id: analogitems
                name: "altimeter_analog"

                OptionRadioButton { text: "Current Height";      }
                OptionRadioButton { text: "Minimum Height";      }
                OptionRadioButton { text: "Maximum Height";      }
                OptionRadioButton { text: "Average Height";      }
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
                name: "altimeter_top"

                OptionRadioButton { text: "Current Height";      }
                OptionRadioButton { text: "Minimum Height";      }
                OptionRadioButton { text: "Maximum Height";      }
                OptionRadioButton { text: "Average Height";      }
                OptionRadioButton { text: "Trip Ascent";         }
                OptionRadioButton { text: "Trip Descent";        }
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
                name: "altimeter_bottom"

                OptionRadioButton { text: "Current Height";      }
                OptionRadioButton { text: "Minimum Height";      }
                OptionRadioButton { text: "Maximum Height";      }
                OptionRadioButton { text: "Average Height";      }
                OptionRadioButton { text: "Trip Ascent";         }
                OptionRadioButton { text: "Trip Descent";        }
            }

            onClicked: {
                console.log("bottomitems.onClicked",index)
                bottomitems.ticked = index
            }
        }
    }
    onConfirm: {
        console.log("AltitudeOptionsPage.onConfirm")
        optionsChanged()
        pageStack.pop()
    }
}
