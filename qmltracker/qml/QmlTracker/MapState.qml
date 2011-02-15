import QtQuick 1.0

State {
    PropertyChanges { target: map;         x:0; y:0; width: parent.width; height: parent.height; }
    PropertyChanges { target: compass;     x:0-width;      }
    PropertyChanges { target: clock;       x:0-width;      }
    PropertyChanges { target: speedometer; x:0-width;      }
    PropertyChanges { target: satellites;  x:parent.width; }
    PropertyChanges { target: levels;      x:parent.width; }
    PropertyChanges { target: altimeter;   x:parent.width; }
}
