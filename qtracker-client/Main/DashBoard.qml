import QtQuick 1.0
import "../Components"
import "../Gauges"

Item {
    id: root
    property bool hide: false
    height: hide? bgimage.height : (landscape? 70*h: 200*w)

    signal clockOptions()
    signal compassOptions()
    signal satsOptions()
    signal speedOptions()
    signal altitudeOptions()

    Settings { id: settings }

    Image {
        id: bgimage
        source: landscape? "dash-bg-landscape.ico": "dash-bg-portrait.ico"
        anchors.bottom: parent.bottom
        height: sourceSize.height*w
        width: sourceSize.width*w
    }

    property list<Item> gaugelist: [
        Clock       { id: clock;     anchors.fill: parent; onOptions: clockOptions()    },
        Compass     { id: compass;   anchors.fill: parent; onOptions: compassOptions()  },
        Satellites  { id: sats;      anchors.fill: parent; onOptions: satsOptions()     },
        Speedometer { id: speed;     anchors.fill: parent; onOptions: speedOptions()    },
        Altimeter   { id: altimeter; anchors.fill: parent; onOptions: altitudeOptions() }
    ]

    property int topleft:     0
    property int centertop:   1
    property int topright:    2
    property int bottomleft:  3
    property int bottomright: 4

    Gauge {
        x:      landscape? 5*w: 0*w
        y:      landscape? parent.height-80*w: parent.height-200*w
        width:  80*w
        height: 80*w
        gauge:  gaugelist[topleft]
        onClicked: { console.log("topleft.onClicked"); var t = centertop; centertop=topleft; topleft=t; }
    }
    Gauge {
        x:      landscape?               440*w:                80*w
        y:      landscape? parent.height-200*w: parent.height-275*w
        width:  198*w
        height: 198*w
        gauge:  gaugelist[centertop]
    }
    Gauge {
        x:      landscape? 365*w: 280*w
        y:      landscape? parent.height-85*w: parent.height-200*w
        width:  80*w
        height: 80*w
        gauge:  gaugelist[topright]
        onClicked: { console.log("topright.onClicked"); var t = centertop; centertop=topright; topright=t; }
    }
    Gauge {
        x:      0
        y:      landscape? parent.height-205*w: parent.height-120*w
        width:  120*w
        height: 120*w
        gauge:  gaugelist[bottomleft]
        onClicked: { console.log("bottomleft.onClicked"); var t = centertop; centertop=bottomleft; bottomleft=t; }
    }
    Gauge {
        x:      landscape? 90*w: 240*w
        y:      parent.height-120*w
        width:  120*w
        height: 120*w
        gauge:  gaugelist[bottomright]
        onClicked: { console.log("bottomright.onClicked"); var t = centertop; centertop=bottomright; bottomright=t; }
    }

    Image {
        source: landscape? "dash-fg-landscape.ico": "dash-fg-portrait.ico"
        anchors.bottom: parent.bottom
        height: sourceSize.height*w
        width: sourceSize.width*w
    }

    onTopleftChanged:       settings.setProperty("dash_topleft",     topleft)
    onCentertopChanged:     settings.setProperty("dash_centertop",   centertop)
    onToprightChanged:      settings.setProperty("dash_topright",    topright)
    onBottomleftChanged:    settings.setProperty("dash_bottomleft",  bottomleft)
    onBottomrightChanged:   settings.setProperty("dash_bottomright", bottomright)

    Component.onCompleted: {
        topleft     = settings.getProperty("dash_topleft",     0)
        centertop   = settings.getProperty("dash_centertop",   1)
        topright    = settings.getProperty("dash_topright",    2)
        bottomleft  = settings.getProperty("dash_bottomleft",  3)
        bottomright = settings.getProperty("dash_bottomright", 4)
    }
}
