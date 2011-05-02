import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: index == -1? "New Track" : "Resume Track"
    options: trkoptions
    confirmbutton: true
    property int index: -1

    Settings {
        id: settings
    }

    VisualItemModel {
        id: trkoptions

        OptionList {
            id: trkbox
            title: "Track Options"
            items: trkitems

            DynamicItemModel {
                id: trkitems
                name: "trkitems"

                OptionInputItem { id: trkname; text: "Name:"; onValueChanged: settings.setProperty("track_defaultname",value) }
                Component.onCompleted: {
                    trkname.value = settings.getProperty("track_defaultname","trk-000")
                }
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
                OptionRadioButton { id: btndist; text: "Distance"; }
                function select(index) {
                    settings.setProperty("track_intervaltype",index)
                    for (var i=0; i<count(); i++) {
                        get(i).ticked = (index == i)
                    }
                }
                Component.onCompleted: {
                    select(settings.getProperty("track_intervaltype",2))
                }
            }
            onClicked: {
                console.log("Interval.onClicked",index)
                intervalitems.select(index)
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
                OptionRadioButton { text: "15 seconds";  }
                OptionRadioButton { text: " 1 minute";   }
                OptionRadioButton { text: " 5 minutes";  }
                OptionRadioButton { text: "15 minutes";  }
                OptionRadioButton { text: " 1 hour";     }

                function select(index) {
                    settings.setProperty("track_intervaltime",index)
                    for (var i=0; i<count(); i++) {
                        get(i).ticked = (index == i)
                    }
                }
                Component.onCompleted: {
                    select(settings.getProperty("track_intervaltime",1))
                }
            }
            onClicked: {
                console.log("Time.onClicked",index)
                timeitems.select(index)
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
                OptionRadioButton { text: " 30 meters";    }
                OptionRadioButton { text: "100 meters";    }
                OptionRadioButton { text: "300 meters";    }
                OptionRadioButton { text: "  1 kilometer"; }

                function select(index) {
                    settings.setProperty("track_intervaldist",index)
                    for (var i=0; i<count(); i++) {
                        get(i).ticked = (index == i)
                    }
                }
                Component.onCompleted: {
                    select(settings.getProperty("track_intervaldist",1))
                }
            }
            onClicked: {
                console.log("Dist.onClicked",index)
                distitems.select(index)
            }
            onVisibleChanged: root.layoutPage();
        }
    }
    onConfirm: {
        console.log("TrackRecordingPage.onConfirm")
        pageStack.pop()
    }
}
