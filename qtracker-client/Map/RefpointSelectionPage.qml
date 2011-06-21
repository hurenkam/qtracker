import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"
import "../Main"

MainOptionPage {
    id: root
    title: "Calibration Points"
    options: refoptions
    //property MapView mapview: null
    //property alias mapid: lst.mapid
    onMapidChanged:   console.log("RefpointSelectionPage.onMapidChanged(",mapid,")")
    onMapnameChanged: console.log("RefpointSelectionPage.onMapnameChanged(",mapname,")")

    RefpointEditPage {
        id:      refedit;
        mapid:   root.mapid
        mapname: root.mapname
        maplat:  root.maplat
        maplon:  root.maplon
        mapx:    root.mapx
        mapy:    root.mapy
        //mapview: root.mapview
        onRefpointChanged: {
            lst.saveRefpoint(index,mapid,name,lat,lon,x,y)
        }
    }

    VisualItemModel {
        id: refoptions

        RefpointList {
            id: lst
            mapid: root.mapid
            //title: "Waypoint List"
            onEditRefpoint:   {
                refedit.index =    index;
                refedit.mappt =    mappt;
                refedit.editname = name;
                refedit.editlat =  latitude;
                refedit.editlon =  longitude;
                refedit.editx =    x;
                refedit.edity =    y;
                pageStack.push(refedit);
            }
            onAddRefpoint: {
                refedit.index =   -1;
                refedit.editname = root.mapname;
                refedit.editlat =  root.maplat;
                refedit.editlon =  root.maplon;
                refedit.editx =    root.mapx;
                refedit.edity =    root.mapy;
                pageStack.push(refedit);
            }
        }
    }
}
