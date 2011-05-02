import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"
import "../Map"

OptionPage {
    id: root
    title: "Route List"
    options: rteoptions
    property MapView mapview: null

    RouteEditPage { id: rteedit; mapview: root.mapview; onRouteSaved: lst.saveRoute(index,name,top,left,bottom,right) }

    VisualItemModel {
        id: rteoptions

        RouteList {
            id: lst
            //title: "Route List"
            //onEditRoute: { rteedit.index = index; pageStack.push(rteedit); }
            onRouteSelected:   {
                rteedit.index = index;
                if (index >= 0) {
                    rteedit.routeid = routeid;
                    rteedit.name = name;
                    rteedit.rtetop = top;
                    rteedit.rteleft = left;
                    rteedit.rtebottom = bottom;
                    rteedit.rteright = right;
                }
                pageStack.push(rteedit);
            }
        }
    }
}
