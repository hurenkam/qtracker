import QtQuick 1.0
import "../components"
import "../model"

TabOptionPage {
    id: root
    title: "Clock Options"
    showheader: false
    signal optionsChanged()
    property alias analogindex: analogitems.ticked
    property alias topindex:    topitems.ticked
    property alias bottomindex: bottomitems.ticked
    property bool landscape: (width>height)
    property Item mapview

    tabs: TabLayout {
        //anchors.fill: parent
        x:      landscape?             gauge.height-15 : 0;
        y:      landscape?                           0 : gauge.width-15
        width:  landscape? gauge.width-gauge.height+15 : gauge.width
        height: landscape?                gauge.height : gauge.height-gauge.width+15

        TabItem {
            title: "Analog"

            OptionList {
                id: analogbox
                //title: "Analog"
                items: analogitems
                x: 0
                y: 0
                width: parent.width

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
        }
        TabItem {
            title: "Top"

            OptionList {
                id: topbox
                //title: "Top"
                items: topitems
                x: 0
                y: 0
                width: parent.width

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
        }
        TabItem {
            title: "Bottom"

            OptionList {
                id: bottombox
                //title: "Bottom"
                items: bottomitems
                x: 0
                y: 0
                width: parent.width

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
/*        TabItem {
            title: "Options"

            OptionList {
                id: sourcebox
                //title: "Source"
                items: sourceitems
                x: 0
                y: 0
                width: parent.width

                RadioBox {
                    id: sourceitems
                    name: "sourceitems"

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
                //title: "Orientation"
                items: orientationitems
                x: 0
                y: sourcebox.height
                width: parent.width

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
*/
    }

    background: Rectangle {
        id: gauge
        objectName: "gauge"
        anchors.fill: parent
        property double margin: root.landscape? (height-radius)/2 : (width-radius)/2
        property double radius: root.landscape? height*0.9 : width*0.9

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.darker(activePalette.light)
            }
            GradientStop {
                position:  1.0
                color: Qt.darker(activePalette.dark)
            }
        }

        Clock {
            id: clock;
            objectName: "clock"
            x: gauge.margin
            y: gauge.margin
            width: gauge.radius
            height: gauge.radius
        }

        ToolButton {
            id: leftbutton
            x: 10; y:10
            width: root.width/7
            height: width

            bgcolor: "black"
            source: "backc.png";
            onClicked: root.cancel();
        }

        ToolButton {
            id: rightbutton

            x: root.width - 10 -width; y:10
            width: root.width/7
            height: width

            source: "confirmc.png";
            bgcolor: "black"
            //onClicked: root.optionsChanged();

            onClicked: {
                console.log("ClockOptionsPage.onConfirm")
                optionsChanged()
                pageStack.pop()
            }

        }
    }
}
