import QtQuick 1.0
import com.nokia.symbian 1.0
import Qt.labs.components 1.0

// Note: This layout is based on a N97 640x360 resolution.
//       With the f factor i try to provide a decent view on devices
//       that have a different resolution.
//       This seems to work fine on all simulated types currently in the SDK
//       (symbian non-touch/non-touch qwerty, symbian touch, freemantle)

Item {
    id: root
    state: "normal"
    property bool landscape: (parent.width>=parent.height)
    property real f: landscape? parent.width/640 : parent.height/640
    property alias mapview: map

    ListModel {
        id: normal
        ListElement { name: "/images/compass";     x: 0;   y:0;   w:120; h:120 }
        ListElement { name: "/images/clock";       x: 0;   y:120; w:120; h:120 }
        ListElement { name: "/images/speedometer"; x: 0;   y:240; w:120; h:120 }
        ListElement { name: "/images/map";         x: 120; y:0;   w:400; h:360 }
        ListElement { name: "/images/satellites";  x: 520; y:0;   w:120; h:120 }
        ListElement { name: "/images/levels";      x: 520; y:120; w:120; h:120 }
        ListElement { name: "/images/altimeter";   x: 520; y:240; w:120; h:120 }
    }
    ListModel {
        id: compasszoom
        ListElement { name: "compass";     x: 170; y:0;   w:360; h:360 }
        ListElement { name: "clock";       x: 0;   y:0;   w:180; h:180 }
        ListElement { name: "speedometer"; x: 0;   y:180; w:180; h:180 }
        ListElement { name: "map";         x: 120; y:680; w:400; h:360 }
        ListElement { name: "satellites";  x: 520; y:0;   w:120; h:120 }
        ListElement { name: "levels";      x: 540; y:130; w:100; h:100 }
        ListElement { name: "altimeter";   x: 520; y:240; w:120; h:120 }
    }
    ListModel {
        id: clockzoom
        ListElement { name: "compass";     x: 0;   y:0;   w:180; h:180 }
        ListElement { name: "clock";       x: 170; y:0;   w:360; h:360 }
        ListElement { name: "speedometer"; x: 0;   y:180; w:180; h:180 }
        ListElement { name: "map";         x: 120; y:680; w:400; h:360 }
        ListElement { name: "satellites";  x: 520; y:0;   w:120; h:120 }
        ListElement { name: "levels";      x: 540; y:130; w:100; h:100 }
        ListElement { name: "altimeter";   x: 520; y:240; w:120; h:120 }
    }
    ListModel {
        id: speedzoom
        ListElement { name: "compass";     x: 0;   y:0;   w:180; h:180 }
        ListElement { name: "clock";       x: 0;   y:180; w:180; h:180 }
        ListElement { name: "speedometer"; x: 170; y:0;   w:360; h:360 }
        ListElement { name: "map";         x: 120; y:680; w:400; h:360 }
        ListElement { name: "satellites";  x: 520; y:0;   w:120; h:120 }
        ListElement { name: "levels";      x: 540; y:130; w:100; h:100 }
        ListElement { name: "altimeter";   x: 520; y:240; w:120; h:120 }
    }
    ListModel {
        id: mapzoom
        ListElement { name: "compass";     x:-160; y:0;   w:120; h:120 }
        ListElement { name: "clock";       x:-160; y:120; w:120; h:120 }
        ListElement { name: "speedometer"; x:-160; y:240; w:120; h:120 }
        ListElement { name: "map";         x:   0; y:0;   w:640; h:360 }
        ListElement { name: "satellites";  x: 680; y:0;   w:120; h:120 }
        ListElement { name: "levels";      x: 680; y:120; w:120; h:120 }
        ListElement { name: "altimeter";   x: 680; y:240; w:120; h:120 }
    }
    ListModel {
        id: satzoom
        ListElement { name: "compass";     x: 0;   y:0;   w:120; h:120 }
        ListElement { name: "clock";       x: 0;   y:130; w:100; h:100 }
        ListElement { name: "speedometer"; x: 0;   y:240; w:120; h:120 }
        ListElement { name: "map";         x: 120; y:680; w:400; h:360 }
        ListElement { name: "satellites";  x: 110; y:0;   w:360; h:360 }
        ListElement { name: "levels";      x: 460; y:0;   w:180; h:180 }
        ListElement { name: "altimeter";   x: 460; y:180; w:180; h:180 }
    }
    ListModel {
        id: levelzoom
        ListElement { name: "compass";     x: 0;   y:0;   w:120; h:120 }
        ListElement { name: "clock";       x: 0;   y:130; w:100; h:100 }
        ListElement { name: "speedometer"; x: 0;   y:240; w:120; h:120 }
        ListElement { name: "map";         x: 120; y:680; w:400; h:360 }
        ListElement { name: "satellites";  x: 460; y:0;   w:180; h:180 }
        ListElement { name: "levels";      x: 110; y:0;   w:360; h:360 }
        ListElement { name: "altimeter";   x: 460; y:180; w:180; h:180 }
    }
    ListModel {
        id: altzoom
        ListElement { name: "compass";     x: 0;   y:0;   w:120; h:120 }
        ListElement { name: "clock";       x: 0;   y:130; w:100; h:100 }
        ListElement { name: "speedometer"; x: 0;   y:240; w:120; h:120 }
        ListElement { name: "map";         x: 120; y:680; w:400; h:360 }
        ListElement { name: "satellites";  x: 460; y:0;   w:180; h:180 }
        ListElement { name: "levels";      x: 460; y:180; w:180; h:180 }
        ListElement { name: "altimeter";   x: 110; y:0;   w:360; h:360 }
    }
    property ListModel layout: normal

    function gaugeX(viewid) { return landscape? layout.get(viewid).x * f: layout.get(viewid).y * f }
    function gaugeY(viewid) { return landscape? layout.get(viewid).y * f: layout.get(viewid).x * f }
    function gaugeW(viewid) { return landscape? layout.get(viewid).w * f: layout.get(viewid).h * f }
    function gaugeH(viewid) { return landscape? layout.get(viewid).h * f: layout.get(viewid).w * f }

    Compass     { id: compass;     viewid: 0; onClicked: root.state == "compass"?     root.state="normal" : root.state="compass"; }
    Clock       { id: clock;       viewid: 1; onClicked: root.state == "clock"?       root.state="normal" : root.state="clock"; }
    Speedometer { id: speedometer; viewid: 2; onClicked: root.state == "speedometer"? root.state="normal" : root.state="speedometer"; }
    MapWrapper  { id: map;         viewid: 3; onLongTap: root.state == "map"?         root.state="normal" : root.state="map"; }
    Satellites  { id: satellites;  viewid: 4; onClicked: root.state == "satellites"?  root.state="normal" : root.state="satellites"; }
    Levels      { id: levels;      viewid: 5; onClicked: root.state == "levels"?      root.state="normal" : root.state="levels"; }
    Altimeter   { id: altimeter;   viewid: 6; onClicked: root.state == "altimeter"?   root.state="normal" : root.state="altimeter"; }

    states: [
        State { name: "normal";      PropertyChanges { target: root; layout: normal      } },
        State { name: "map";         PropertyChanges { target: root; layout: mapzoom     } },
        State { name: "compass";     PropertyChanges { target: root; layout: compasszoom } },
        State { name: "clock";       PropertyChanges { target: root; layout: clockzoom   } },
        State { name: "speedometer"; PropertyChanges { target: root; layout: speedzoom   } },
        State { name: "satellites";  PropertyChanges { target: root; layout: satzoom     } },
        State { name: "levels";      PropertyChanges { target: root; layout: levelzoom   } },
        State { name: "altimeter";   PropertyChanges { target: root; layout: altzoom     } }
    ]

    Component.onCompleted: console.log("size: ",width,height)
}
