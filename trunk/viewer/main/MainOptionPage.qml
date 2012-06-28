import QtQuick 1.0
import "../components"

OptionPage {
    id: root
    property int    mapid:   -1
    property string mapname: "<no map>"
    property double maplat:  0.0
    property double maplon:  0.0
    property double mapalt:  0.0
    property int    mapx:    0
    property int    mapy:    0

    function setupMapArguments(id, name, lat, lon, alt, x, y) {
        console.log("MainOptionPage.setupMapArguments(",id,name,lat,lon,alt,x,y,")")
        mapid = id
        mapname = name
        maplat = lat
        maplon = lon
        mapalt = alt
        mapx = x
        mapy = y
    }
}
