import QtQuick 1.0
import QmlTrackerExtensions 1.0
import "../components"

OptionPage {
    id: root
    title: (refid==-1) ? "New Refpoint" : "Edit Refpoint"
    options: refoptions
    confirmbutton: true
    property int refid: -1
    property TRefpoint dbrecord

    signal refpointSaved(int refid)

    Settings {
        id: settings
    }

    VisualItemModel {
        id: refoptions

        OptionList {
            id: refbox
            items: refitems

            DynamicItemModel {
                id: refitems
                name: "refitems"

                OptionInputItem { id: refname;        text: "Name:     "; onValueChanged: settings.setProperty("refpt_defaultname",value) }
                OptionInputItem { id: reflat;         text: "Latitude: "; value: "0.0" }
                OptionInputItem { id: reflon;         text: "Longitude:"; value: "0.0" }
                OptionInputItem { id: refx;           text: "X:        "; value: "0" }
                OptionInputItem { id: refy;           text: "Y:        "; value: "0" }

                Component.onCompleted: {
                    refname.value = settings.getProperty("refpt_defaultname","refpt-000")
                }
            }
        }
    }

    function refreshData() {
        if (refid==-1) {
            refname.value = settings.getProperty("refpt_defaultname","refpt-000")
            reflat.value  = 0.0
            reflon.value  = 0.0
            refx.value = 0
            refy.value = 0
        } else {
            dbrecord = database.getRefpoint(refid)
            refname.value = dbrecord.name
            reflat.value = dbrecord.latitude
            reflon.value = dbrecord.longitude
            refx.value = dbrecord.x
            refy.value = dbrecord.y
        }
    }

    function saveWaypoint() {
        if (refid==-1) {
            dbrecord = database.getRefpoint(refid)
        }

        dbrecord.name = refname.value
        dbrecord.latitude = reflat.value
        dbrecord.longitude = reflon.value
        dbrecord.x = refx.value
        dbrecord.y = refy.value
        dbrecord.save()
        root.refpointSaved(dbrecord.mappt);

        refid = dbrecord.mappt
    }

    onRefidChanged: refreshData()
    Component.onCompleted: refreshData()
    onConfirm: {
        console.log("RefpointEditPage.onConfirm",refname.value,reflat.value,reflon.value,refx.value,refy.value)
        saveRefpoint()
        pageStack.pop()
    }
}
