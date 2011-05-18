import QtQuick 1.0
import "../Components"

OptionPage {
    id: root
    title: index == -1? "New Trip" : "Edit Trip"
    options: tripoptions
    confirmbutton: true
    property int index: -1
    property alias name: tripname.value

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
            title: "Trip Options"
            items: tripitems

            DynamicItemModel {
                id: tripitems
                name: "tripitems"

                OptionInputItem { id: tripname; text: "Name:"; onValueChanged: settings.setProperty("trip_defaultname",value) }
                Component.onCompleted: {
                    tripname.value = settings.getProperty("trip_defaultname","trip-000")
                }
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
