import QtQuick 1.0

Rectangle {
    id: main
    SystemPalette { id: activePalette }
    color: activePalette.dark

    Layout { id: view; }
/*
    states: [
        State {
            name: "map"
            PropertyChanges { target: view.map;         x:0; y:0; width: parent.width; height: parent.height; }
            PropertyChanges { target: view.compass;     x:0-width;      }
            PropertyChanges { target: view.clock;       x:0-width;      }
            PropertyChanges { target: view.speedometer; x:0-width;      }
            PropertyChanges { target: view.satellites;  x:parent.width; }
            PropertyChanges { target: view.levels;      x:parent.width; }
            PropertyChanges { target: view.altimeter;   x:parent.width; }
        },
        //MapState { name: "map" },
        State {
            name: "compass"
            PropertyChanges { target: view.map;         y: parent.width+10 }
            PropertyChanges { target: view.compass;     y:0; x: parent.height/2-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: view.clock;       y:0; x: 0;               width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.speedometer; y:parent.height/2; x: 0; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.levels;      y:parent.height/3+5; x: parent.width - parent.height/3 +10; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "clock"
            PropertyChanges { target: view.map;         y: parent.width+10 }
            PropertyChanges { target: view.clock;       y:0; x: parent.height/2-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: view.compass;     y:0; x: 0;               width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.speedometer; y:parent.height/2; x: 0; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.levels;      y:parent.height/3+5; x: parent.width - parent.height/3 +10; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "speedometer"
            PropertyChanges { target: view.map;         y: parent.width+10 }
            PropertyChanges { target: view.speedometer; y:0; x: parent.height/2-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: view.compass;     y:0; x: 0;               width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.clock;       y:parent.height/2; x: 0; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.levels;      y:parent.height/3+5; x: parent.width - parent.height/3 +10; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "satellites"
            PropertyChanges { target: view.map;         y: parent.width+10 }
            PropertyChanges { target: view.satellites;  y:0; x: parent.height/3-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: view.levels;      y:0;                 x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.altimeter;   y:parent.height/2;   x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.clock;       y:parent.height/3+5; x: 0; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "levels"
            PropertyChanges { target: view.map;         y: parent.width+10 }
            PropertyChanges { target: view.levels;      y:0; x: parent.height/3-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: view.satellites;  y:0;                 x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.altimeter;   y:parent.height/2;   x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.clock;       y:parent.height/3+5; x: 0; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "altimeter"
            PropertyChanges { target: view.map;         y: parent.width+10 }
            PropertyChanges { target: view.altimeter;   y:0; x: parent.height/3-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: view.satellites;  y:0;                 x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.levels;      y:parent.height/2;   x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: view.clock;       y:parent.height/3+5; x: 0; width: parent.height/3-10; height: parent.height/3-10; }
        }
    ]
    //states: Landscape { w: parent.width; h: parent.height }

    transitions: [
        Transition {
            from: "*"; to: "*"
            NumberAnimation {
                properties: "x,y,width,height"
                easing.type: Easing.InOutQuart
                duration: 800
            }
        }
    ]

    Connections { target: view.map;         onLongTap: main.state == "map"         ? main.state="" : main.state="map";         }
    Connections { target: view.compass;     onClicked: main.state == "compass"     ? main.state="" : main.state="compass";     }
    Connections { target: view.clock;       onClicked: main.state == "clock"       ? main.state="" : main.state="clock";       }
    Connections { target: view.speedometer; onClicked: main.state == "speedometer" ? main.state="" : main.state="speedometer"; }
    Connections { target: view.satellites;  onClicked: main.state == "satellites"  ? main.state="" : main.state="satellites";  }
    Connections { target: view.levels;      onClicked: main.state == "levels"      ? main.state="" : main.state="levels";      }
    Connections { target: view.altimeter;   onClicked: main.state == "altimeter"   ? main.state="" : main.state="altimeter";   }
    Connections { target: view.map; onMenuClicked:     console.log("menu not yet implemented")         }
    Connections { target: view.map; onWaypointClicked: console.log("waypoint not yet implemented")     }
    Connections { target: view.map; onRouteClicked:    console.log("route not yet implemented")        }
    Connections { target: view.map; onTrackClicked:    console.log("track not yet implemented")        }
    Connections { target: view.map; onLoadClicked:     console.log("load not yet implemented")         }
    Connections { target: view.map; onSaveClicked:     console.log("save not yet implemented")         }
*/
}
