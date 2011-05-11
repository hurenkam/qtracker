import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: "Clock Options"
    options: clockoptions
    confirmbutton: true
    signal optionsChanged()

    VisualItemModel {
        id: clockoptions

        OptionList {
            id: analogbox
            title: "Analog"
            items: analogitems

            DynamicItemModel {
                id: analogitems
                name: "analogitems"

                OptionRadioButton { text: "Current Time";      ticked: true }
                OptionRadioButton { text: "Elapsed Time";      }
                OptionRadioButton { text: "Remaining Time";    }
                OptionRadioButton { text: "Monitor Time";      }
            }
            onClicked: {
                console.log("analogitems.onClicked",index)
                for (var i=0; i<analogitems.count(); i++) {
                    analogitems.get(i).ticked = (index == i)
                }
            }
        }
        OptionList {
            id: topbox
            title: "Top"
            items: topitems

            DynamicItemModel {
                id: topitems
                name: "topitems"

                OptionRadioButton { text: "Current Time";      ticked: true }
                OptionRadioButton { text: "Elapsed Time";      }
                OptionRadioButton { text: "Remaining Time";    }
                OptionRadioButton { text: "Monitor Time";      }
            }
            onClicked: {
                console.log("topitems.onClicked",index)
                for (var i=0; i<topitems.count(); i++) {
                    topitems.get(i).ticked = (index == i)
                }
            }
        }
        OptionList {
            id: bottombox
            title: "Bottom"
            items: bottomitems

            DynamicItemModel {
                id: bottomitems
                name: "bottomitems"

                OptionRadioButton { text: "Current Time";      ticked: true }
                OptionRadioButton { text: "Elapsed Time";      }
                OptionRadioButton { text: "Remaining Time";    }
                OptionRadioButton { text: "Monitor Time";      }
            }
            onClicked: {
                console.log("bottomitems.onClicked",index)
                for (var i=0; i<bottomitems.count(); i++) {
                    bottomitems.get(i).ticked = (index == i)
                }
            }
        }
    }
    onConfirm: {
        console.log("ClockOptionsPage.onConfirm")
        optionsChanged()
        pageStack.pop()
    }
}
