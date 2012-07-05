import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../components"

OptionPage {
    id: root
    title: trkid == -1? "New Track" : "Resume Track"
    options: trkoptions
    property int trkid: -1
    leftbuttonsrc: "left-plain.png"
    rightbutton: true
    rightbuttonsrc: "visible-plain.png"
    //rightbuttonsrc: trackstatus.status=="idle"? "../Images/visible-plain.png" : "../Images/stop-plain.png"

    Settings {
        id: settings
    }

    signal trackSaved(int trkid, string name, int interval)

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

            RadioBox {
                id: intervalitems
                name: "trk_interval"

                OptionRadioButton { id: btnall;  text: "All";      }
                OptionRadioButton { id: btntime; text: "Time";     }
                OptionRadioButton { id: btndist; text: "Distance"; }
            }
            onClicked: {
                console.log("Interval.onClicked",index)
                intervalitems.ticked=index
            }
        }

        OptionList {
            id: timeoptions
            title: "Time Interval"
            items: timeitems
            visible: btntime.ticked

            RadioBox {
                id: timeitems
                name: "trk_timeinterval"

                OptionRadioButton { text: " 5 seconds";  }
                OptionRadioButton { text: "15 seconds";  }
                OptionRadioButton { text: " 1 minute";   }
                OptionRadioButton { text: " 5 minutes";  }
                OptionRadioButton { text: "15 minutes";  }
                OptionRadioButton { text: " 1 hour";     }
            }
            onClicked: {
                console.log("Time.onClicked",index)
                timeitems.ticked=index
            }
            onVisibleChanged: root.layoutPage();
        }

        OptionList {
            id: distoptions
            title: "Distance Interval"
            items: distitems
            visible: btndist.ticked

            RadioBox {
                id: distitems
                name: "trk_distinterval"

                OptionRadioButton { text: " 10 meters";    }
                OptionRadioButton { text: " 30 meters";    }
                OptionRadioButton { text: "100 meters";    }
                OptionRadioButton { text: "300 meters";    }
                OptionRadioButton { text: "  1 kilometer"; }
            }
            onClicked: {
                console.log("Dist.onClicked",index)
                distitems.ticked=index
            }
            onVisibleChanged: root.layoutPage();
        }
    }

    XmlRpc {
        id: rpc;
    }

    function startTrack(id,interval) {
        console.log("TrackRecordingPage.startTrack()")
        rpc.call("start",[id,interval])
    }

    function stopTrack(id) {
        console.log("TrackRecordingPage.stopTrack()")
        rpc.call("stop",[])
    }

    function startOrSaveTrack() {
        var times = [ 5, 15, 60, 5*60, 15*60, 60*60 ]
        var time = times[timeitems.ticked]
        var dists = [ 10, 30, 100, 300, 1000 ]
        var dist = dists[distitems.ticked]
        var type = intervalitems.ticked

        var message = "TrackRecordingPage.onConfirm: " + trkname.value
        message += " " + intervalitems.get(type).text
        if (type == 1) message += " " + time + " seconds"
        if (type == 2) message += " " + dist + " meters"
        console.log(message)
        var dbrecord = database.getTrack(root.trkid)
        dbrecord.name = trkname.value
        dbrecord.interval = (type==0)? 0 : ((type==1)? time: -1 * dist)
        dbrecord.save()
        if (root.trkid==-1)
            startTrack(dbrecord.trkid,dbrecord.interval)
        trackSaved(dbrecord.trkid,dbrecord.name,dbrecord.interval)
    }

    onConfirm: {
        startOrSaveTrack()
        pageStack.pop()
    }
}
