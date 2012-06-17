import QtQuick 1.0
import QtMobility.sensors 1.2
import QtMobility.location 1.2
import "../components"
import "../model"

TabOptionPage {
    id: root
    title: "Compass Options"
    showheader: false
    signal optionsChanged()
    property alias sourceindex:      sourceitems.ticked
    property alias orientationindex: orientationitems.ticked
    property bool landscape: (width>height)
    property Item mapview

    tabs: TabLayout {
        //anchors.fill: parent
        x:      landscape?             gauge.height-15 : 0;
        y:      landscape?                           0 : gauge.width-15
        width:  landscape? gauge.width-gauge.height+15 : gauge.width
        height: landscape?                gauge.height : gauge.height-gauge.width+15

        TabItem {
            title: "Face"

            OptionList {
                id: facesrcbox
                title: "Source"
                items: facesrcitems
                x: 0
                y: 0
                width: parent.width

                RadioBox {
                    id: facesrcitems
                    name: "compass_facesrc"

                    OptionRadioButton { text: "North";   }
                    OptionRadioButton { text: "Heading"; }
                    OptionRadioButton { text: "Monitor"; }
                }
                onClicked: {
                    console.log("facesrcitems.onClicked",index)
                    facesrcitems.ticked = index
                }
            }
        }
        TabItem {
            title: "Needle"

            OptionList {
                id: needlesrcbox
                title: "Source"
                items: needlesrcitems
                x: 0
                y: 0
                width: parent.width

                RadioBox {
                    id: needlesrcitems
                    name: "compass_needlesrc"

                    OptionRadioButton { text: "North";   }
                    OptionRadioButton { text: "Heading"; }
                    OptionRadioButton { text: "Monitor"; }
                }
                onClicked: {
                    console.log("facesrcitems.onClicked",index)
                    needlesrcitems.ticked = index
                }
            }
        }
        TabItem {
            title: "Ring"

            OptionList {
                id: ringsrcbox
                title: "Source"
                items: ringsrcitems
                x: 0
                y: 0
                width: parent.width

                RadioBox {
                    id: ringsrcitems
                    name: "compass_ringsrc"

                    OptionRadioButton { text: "North";   }
                    OptionRadioButton { text: "Heading"; }
                    OptionRadioButton { text: "Monitor"; }
                }
                onClicked: {
                    console.log("ringsrcitems.onClicked",index)
                    ringsrcitems.ticked = index
                }
            }
        }
        TabItem {
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
    }

    PositionSource {
        id: gps
        updateInterval: 1000
        active: true
        onPositionChanged: {
            if (gps.position.altitudeValid) altitude.value = gps.position.coordinate.altitude
            latitude.value  = position.coordinate.latitude
            longitude.value = position.coordinate.longitude
        }
    }

    Compass {
        id: sensor
        //dataRate: 4
        Component.onCompleted: sensor.start()
        //onReadingChanged: azimuth.value = reading.azimuth
    }
    Timer {
        id: currenttimer; interval: 250; running: true; repeat: true;
        onTriggered: azimuth.value = sensor.reading.azimuth
    }
    CourseModel {
        id: coursemodel
        interval: 5000
    }

    Item { id: latitude;  property double value: 0.0 }
    Item { id: longitude; property double value: 0.0 }
    Item { id: altitude;  property double value: 0.0 }

    Item { id: azimuth;   property double value: 0.0 }

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

        Compass2 {
            id: compass
            objectName: "compass"
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
                console.log("CompassOptionsPage.onConfirm")
                optionsChanged()
                pageStack.pop()
            }

        }

        OptionInputItem {
            id: azimuthitem
            anchors.top: compass.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Azimuth: ";
            value: azimuth.value.toFixed(1);
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }

        OptionInputItem {
            id: headingitem
            anchors.top: azimuthitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Heading: ";
            value: coursemodel.average.toFixed(1);
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
        OptionInputItem {
            id: bearingitem
            anchors.top: headingitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Bearing: ";
            value: course.monitor.toFixed(1);
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
        OptionInputItem {
            id: latitudeitem
            anchors.top: bearingitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Latitude: ";
            value: latitude.value.toFixed(5);
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
        OptionInputItem {
            id: longitudeitem
            anchors.top: latitudeitem.bottom
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.right:  parent.right
            text: "Longitude: ";
            value: longitude.value.toFixed(5);
            textcolor: "white"
            valuecolor: "yellow"
            readOnly: true
            underline: false
        }
    }
}
