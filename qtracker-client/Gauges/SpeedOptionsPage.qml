import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

TabOptionPage {
    id: root
    title: "Speed Options"
    confirmbutton: true
    showheader: false
    property bool landscape: (width>height)
    property Item mapview
    property alias analogindex: analogitems.ticked
    property alias topindex:    topitems.ticked
    property alias bottomindex: bottomitems.ticked

    signal optionsChanged()

    tabs: TabLayout {
        //anchors.fill: parent
        x:      landscape?             gauge.height-15 : 0;
        y:      landscape?                           0 : gauge.width-75
        width:  landscape? gauge.width-gauge.height+15 : gauge.width
        height: landscape?                gauge.height : gauge.height-gauge.width+75

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
        TabItem {
            title: "Options"

            OptionList {
                id: optionbox
                //title: "Source"
                items: optionitems
                x: 0
                y: 0
                width: parent.width

                DynamicItemModel {
                    id: optionitems
                    name: "optionitems"

                    OptionInputItem { text: "Average buffer size:"; value: "60" }
                    OptionInputItem { text: "Distance hysteresis:"; value: "30" }
                }
            }
        }
    }

    ValueSpaceSubscriber { id: speedcur; path: "/server/speed/current"     }
    ValueSpaceSubscriber { id: speedmin; path: "/server/speed/min"         }
    ValueSpaceSubscriber { id: speedmax; path: "/server/speed/max"         }
    ValueSpaceSubscriber { id: speedavg; path: "/server/speed/average"     }
    ValueSpaceSubscriber { id: distance; path: "/server/location/distance" }
    ValueSpaceSubscriber { id: monitor;  path: "/server/monitor/distance"  }

    background: Rectangle {
        id: gauge
        objectName: "gauge"
        anchors.fill: parent
        property double margin: root.landscape? (height-radius)/2 : (width-radius)/2
        property double radius: root.landscape? height*0.9 : width*0.9

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.lighter(activePalette.light)
            }
            GradientStop {
                position:  1.0
                color: Qt.lighter(activePalette.dark)
            }
        }

        Speedometer {
            id: speedometer;
            objectName: "speedometer"
            x: gauge.margin
            y: gauge.margin
            width: gauge.radius
            height: gauge.radius
        }

        ToolButton {
            id: leftbutton
            x: 10; y:10
            width: 50
            height: width

            bgcolor: "white"
            source: "backc.svg";
            onClicked: root.cancel();
        }

        ToolButton {
            id: rightbutton

            x: landscape? root.height -10 -width: root.width - 10 -width; y:10
            width: 50
            height: width

            source: "confirmc.svg";
            bgcolor: "white"
            //onClicked: root.optionsChanged();

            onClicked: {
                console.log("SpeedOptionsPage.onConfirm")
                optionsChanged()
                pageStack.pop()
            }
        }
        OptionInputItem {
            id: actualitem
            anchors.top: speedometer.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Actual: ";
            value: speedcur.value;
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
        OptionInputItem {
            id: minitem
            anchors.top: actualitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Minimum: ";
            value: speedmin.value;
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
        OptionInputItem {
            id: maxitem
            anchors.top: minitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Maximum: ";
            value: speedmax.value;
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
        OptionInputItem {
            id: avgitem
            anchors.top: maxitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Average: ";
            value: speedavg.value;
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
        OptionInputItem {
            id: distitem
            anchors.top: avgitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Trip distance: ";
            value: distance.value;
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
        OptionInputItem {
            id: monitem
            anchors.top: distitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Monitor distance: ";
            value: monitor.value;
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
    }
}
