import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: index == -1? "New Track" : "Resume Track"
    options: trkoptions
    confirmbutton: true
    property int index: -1

    VisualItemModel {
        id: trkoptions

        OptionList {
            id: trkbox
            title: "Track Options"
            items: trkitems

            DynamicItemModel {
                id: trkitems
                name: "trkitems"

                OptionInputItem { id: trkname; text: "Name:";   value: "Home" }
            }
        }

        OptionList {
            id: intervaloptions
            title: "Recording Options"
            items: intervalitems

            DynamicItemModel {
                id: intervalitems
                name: "intervalitems"

                OptionRadioButton { id: btnall;  text: "All";      }
                OptionRadioButton { id: btntime; text: "Time";     }
                OptionRadioButton { id: btndist; text: "Distance"; ticked: true }
            }
            onClicked: {
                console.log("Interval.onClicked",index)
                for (var i=0; i<intervalitems.count(); i++) {
                    intervalitems.get(i).ticked = (index == i)
                }
            }
        }

        OptionList {
            id: timeoptions
            title: "Time Interval"
            items: timeitems
            visible: btntime.ticked

            DynamicItemModel {
                id: timeitems
                name: "timeitems"

                OptionRadioButton { text: " 5 seconds";  }
                OptionRadioButton { text: "15 seconds";  ticked: true }
                OptionRadioButton { text: " 1 minute";   }
                OptionRadioButton { text: " 5 minutes";  }
                OptionRadioButton { text: "15 minutes";  }
                OptionRadioButton { text: " 1 hour";     }
            }
            onClicked: {
                console.log("Time.onClicked",index)
                for (var i=0; i<timeitems.count(); i++) {
                    timeitems.get(i).ticked = (index == i)
                }
            }
            onVisibleChanged: root.layoutPage();
        }

        OptionList {
            id: distoptions
            title: "Distance Interval"
            items: distitems
            visible: btndist.ticked

            DynamicItemModel {
                id: distitems
                name: "distitems"

                OptionRadioButton { text: " 10 meters";    }
                OptionRadioButton { text: " 30 meters";    ticked: true }
                OptionRadioButton { text: "100 meters";    }
                OptionRadioButton { text: "300 meters";    }
                OptionRadioButton { text: "  1 kilometer"; }
            }
            onClicked: {
                console.log("Distance.onClicked",index)
                for (var i=0; i<distitems.count(); i++) {
                    distitems.get(i).ticked = (index == i)
                }
            }
            onVisibleChanged: root.layoutPage();
        }
    }
    onConfirm: {
        console.log("TrackRecordingPage.onConfirm")
        pageStack.pop()
    }
}
