import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionPage {
    id: root
    title: "Route List"
    options: rteoptions

    RouteEditPage { id: rteedit }

    VisualItemModel {
        id: rteoptions

        RouteList {
            id: lst
            //title: "Route List"
            onEditRoute: { rteedit.index = index; pageStack.push(rteedit); }
        }
    }
}
