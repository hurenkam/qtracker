import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionPage {
    id: root
    title: "Calibration Points"
    options: refoptions

    RefpointEditPage { id: refedit }

    VisualItemModel {
        id: refoptions

        RefpointList {
            id: lst
            //title: "Waypoint List"
            onEditRefpoint:   { refedit.index = index; pageStack.push(refedit); }
        }
    }
}
