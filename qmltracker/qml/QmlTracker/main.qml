import QtQuick 1.0

Rectangle {
    id: main
    SystemPalette { id: activePalette }
    color: activePalette.dark

    Compass {
        id: compass
        x: 0; y: 0; width: parent.height/3; height: parent.height/3
    }

    Clock {
        id: clock
        x: 0; y: parent.height/3; width: parent.height/3; height: parent.height/3
    }

    Speedometer {
        id: speedometer
        x: 0; y: 2*parent.height/3; width: parent.height/3; height: parent.height/3
    }

    Satellites {
        id: satellites
        x: parent.width - parent.height/3; y: 0; width: parent.height/3; height: parent.height/3
    }

    Levels {
        id: levels
        x: parent.width-parent.height/3; y: parent.height/3; width: parent.height/3; height: parent.height/3
    }

    Altimeter {
        id: altimeter
        x: parent.width-parent.height/3; y: 2*parent.height/3; width: parent.height/3; height: parent.height/3
    }

    MapWrapper {
        id: map
        x: parent.height/3;
        y: 0
        width: parent.width-parent.height/3*2
        height: parent.height
    }

    states: [
        State {
            name: "map"
            PropertyChanges { target: map;         x:0; y:0; width: parent.width; height: parent.height; }
            PropertyChanges { target: compass;     x:0-width;      }
            PropertyChanges { target: clock;       x:0-width;      }
            PropertyChanges { target: speedometer; x:0-width;      }
            PropertyChanges { target: satellites;  x:parent.width; }
            PropertyChanges { target: levels;      x:parent.width; }
            PropertyChanges { target: altimeter;   x:parent.width; }
        },
        State {
            name: "compass"
            PropertyChanges { target: map;         y: parent.width+10 }
            PropertyChanges { target: compass;     y:0; x: parent.height/2-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: clock;       y:0; x: 0;               width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: speedometer; y:parent.height/2; x: 0; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: levels;      y:parent.height/3+5; x: parent.width - parent.height/3 +10; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "clock"
            PropertyChanges { target: map;         y: parent.width+10 }
            PropertyChanges { target: clock;       y:0; x: parent.height/2-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: compass;     y:0; x: 0;               width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: speedometer; y:parent.height/2; x: 0; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: levels;      y:parent.height/3+5; x: parent.width - parent.height/3 +10; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "speedometer"
            PropertyChanges { target: map;         y: parent.width+10 }
            PropertyChanges { target: speedometer; y:0; x: parent.height/2-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: compass;     y:0; x: 0;               width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: clock;       y:parent.height/2; x: 0; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: levels;      y:parent.height/3+5; x: parent.width - parent.height/3 +10; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "satellites"
            PropertyChanges { target: map;         y: parent.width+10 }
            PropertyChanges { target: satellites;  y:0; x: parent.height/3-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: levels;      y:0;                 x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: altimeter;   y:parent.height/2;   x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: clock;       y:parent.height/3+5; x: 0; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "levels"
            PropertyChanges { target: map;         y: parent.width+10 }
            PropertyChanges { target: levels;      y:0; x: parent.height/3-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: satellites;  y:0;                 x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: altimeter;   y:parent.height/2;   x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: clock;       y:parent.height/3+5; x: 0; width: parent.height/3-10; height: parent.height/3-10; }
        },
        State {
            name: "altimeter"
            PropertyChanges { target: map;         y: parent.width+10 }
            PropertyChanges { target: altimeter;   y:0; x: parent.height/3-10; width: parent.height; height: parent.height; }
            PropertyChanges { target: satellites;  y:0;                 x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: levels;      y:parent.height/2;   x: parent.width - parent.height/2; width: parent.height/2; height: parent.height/2; }
            PropertyChanges { target: clock;       y:parent.height/3+5; x: 0; width: parent.height/3-10; height: parent.height/3-10; }
        }
    ]

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

    Connections { target: map;         onLongTap: main.state == "map"         ? main.state="" : main.state="map";         }
    Connections { target: compass;     onClicked: main.state == "compass"     ? main.state="" : main.state="compass";     }
    Connections { target: clock;       onClicked: main.state == "clock"       ? main.state="" : main.state="clock";       }
    Connections { target: speedometer; onClicked: main.state == "speedometer" ? main.state="" : main.state="speedometer"; }
    Connections { target: satellites;  onClicked: main.state == "satellites"  ? main.state="" : main.state="satellites";  }
    Connections { target: levels;      onClicked: main.state == "levels"      ? main.state="" : main.state="levels";      }
    Connections { target: altimeter;   onClicked: main.state == "altimeter"   ? main.state="" : main.state="altimeter";   }
    Connections { target: map; onMenuClicked:     console.log("menu not yet implemented")         }
    Connections { target: map; onWaypointClicked: console.log("waypoint not yet implemented")     }
    Connections { target: map; onRouteClicked:    console.log("route not yet implemented")        }
    Connections { target: map; onTrackClicked:    console.log("track not yet implemented")        }
    Connections { target: map; onLoadClicked:     console.log("load not yet implemented")         }
    Connections { target: map; onSaveClicked:     console.log("save not yet implemented")         }
}
