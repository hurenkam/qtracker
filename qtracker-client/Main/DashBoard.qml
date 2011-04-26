import QtQuick 1.0
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

    Image {
        id: bgimage
        source: landscape? "dash-bg-landscape.ico": "dash-bg-portrait.ico"
        anchors.bottom: parent.bottom
        height: sourceSize.height*w
        width: sourceSize.width*w
    }

    Clock       { id: clock;     anchors.fill: parent; objectName: "clock";     onClicked: root.zoom(clock);     onOptions: clockOptions()    }
    Compass     { id: compass;   anchors.fill: parent; objectName: "compass";   onClicked: root.zoom(compass);   onOptions: compassOptions()  }
    Satellites  { id: sats;      anchors.fill: parent; objectName: "sats";      onClicked: root.zoom(sats);      onOptions: satsOptions()     }
    Speedometer { id: speed;     anchors.fill: parent; objectName: "speed";     onClicked: root.zoom(speed);     onOptions: speedOptions()    }
    Altimeter   { id: altimeter; anchors.fill: parent; objectName: "altimeter"; onClicked: root.zoom(altimeter); onOptions: altitudeOptions() }

    Gauge {
        id:     topleft
        x:      landscape? 5*w: 0*w
        y:      landscape? parent.height-80*w: parent.height-200*w
        width:  80*w
        height: 80*w
        gauge:  clock
    }
    Gauge {
        id:     centertop
        x:      landscape?               440*w:                80*w
        y:      landscape? parent.height-200*w: parent.height-275*w
        width:  198*w
        height: 198*w
        gauge:  compass
    }
    Gauge {
        id:     topright
        x:      landscape? 365*w: 280*w
        y:      landscape? parent.height-85*w: parent.height-200*w
        width:  80*w
        height: 80*w
        gauge:  sats
    }
    Gauge {
        id:     bottomleft
        x:      0
        y:      landscape? parent.height-205*w: parent.height-120*w
        width:  120*w
        height: 120*w
        gauge:  speed
    }
    Gauge {
        id:     bottomright
        x:      landscape? 90*w: 240*w
        y:      parent.height-120*w
        width:  120*w
        height: 120*w
        gauge:  altimeter
    }

    function zoom(source) {
        if (source.objectName == centertop.gauge.objectName) return
        var parent1 = source.parent
        var parent2 = centertop

        console.log("DashBoard.zoom: ",parent1.gauge.objectName,parent2.gauge.objectName)

        var src1 = source
        var src2 = centertop.gauge
        parent1.gauge = src2
        parent2.gauge = src1
    }

    Image {
        source: landscape? "dash-fg-landscape.ico": "dash-fg-portrait.ico"
        anchors.bottom: parent.bottom
        height: sourceSize.height*w
        width: sourceSize.width*w
    }
}
