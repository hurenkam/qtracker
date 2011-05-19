import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: index == -1? "New Trip" : "Edit Trip"
    options: tripoptions
    confirmbutton: true
    property int index: -1

    property variant record: {}

    Settings {
        id: settings
    }

    signal tripSaved(int index, string name)
    signal tripStopped()
    signal tripStarted()

    function saveTrip(index,name) {
        console.log("TripEditPage.saveTrip",index,name)
        tripSaved(index,name)
    }

    VisualItemModel {
        id: tripoptions

        OptionList {
            id: tripbox
            title: "Trip Data"
            items: tripitems

            DynamicItemModel {
                id: tripitems
                name: "tripitems"

                OptionInputItem { id: tripname; text: "Name:    "; value: root.record.name;     onValueChanged: settings.setProperty("trip_defaultname",value) }
                OptionInputItem { id: triptime; text: "Time:    "; value: root.record.triptime; readOnly: true }
                OptionInputItem { id: tripdist; text: "Distance:"; value: root.record.tripdist; readOnly: true }

                Component.onCompleted: {
                    tripname.value = settings.getProperty("trip_defaultname","trip-000")
                }
            }
        }

        OptionList {
            id: speedbox
            title: "Speed Data"
            items: speeditems

            DynamicItemModel {
                id: speeditems
                name: "speeditems"

                OptionInputItem { id: tripavgspeed; text: "Average:"; value: root.record.speedavg; readOnly: true }
                OptionInputItem { id: tripminspeed; text: "Min:";     value: root.record.speedmin; readOnly: true }
                OptionInputItem { id: tripmaxspeed; text: "Max:";     value: root.record.speedmax; readOnly: true }
            }
        }

        OptionList {
            id: altbox
            title: "Altitude Data"
            items: altitems

            DynamicItemModel {
                id: altitems
                name: "altitems"

                OptionInputItem { id: tripascent;  text: "Ascent:";  value: root.record.ascent;  readOnly: true }
                OptionInputItem { id: tripdescent; text: "Descent:"; value: root.record.descent; readOnly: true }
                OptionInputItem { id: tripavgalt;  text: "Average:"; value: root.record.altavg;  readOnly: true }
                OptionInputItem { id: tripminalt;  text: "Min:";     value: root.record.altmin;  readOnly: true }
                OptionInputItem { id: tripmaxalt;  text: "Max:";     value: root.record.altmax;  readOnly: true }
            }
        }

        OptionList {
            id: associationbox
            title: "Associations"
            items: associationitems

            DynamicItemModel {
                id: associationitems
                name: "associationitems"

                OptionTextItem { id: tripwpt; text: "Waypoints"; button: true; onClicked: ; }
                OptionTextItem { id: triprte; text: "Tracks";    button: true; onClicked: ; }
                OptionTextItem { id: triptrk; text: "Routes";    button: true; onClicked: ; }
                OptionTextItem { id: tripmap; text: "Maps";      button: true; onClicked: ; }
            }
        }
    }

    function startTrip(id) {
        var cmd = { "class": "trip", "method": "start", "args": [ id ] }
        console.log("TripEditPage.startTrip(): ",cmd)
        client.sendCommand(cmd);
        tripStarted()
    }

    function stopTrip(id) {
        var cmd = { "class": "trip", "method": "stop", "args": [ id ] }
        console.log("TripEditPage.stopTrip(): ",cmd)
        client.sendCommand(cmd);
        tripStopped()
    }

    onConfirm: {
        var message = "TripEditPage.onConfirm: " + tripname.value
        console.log(message)
        saveTrip(root.index,tripname.value)
        pageStack.pop()
    }
}
