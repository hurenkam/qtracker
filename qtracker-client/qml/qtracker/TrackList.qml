import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

Rectangle {
    id: root
    anchors.margins: 5
    clip:  true
    property alias index: tracklist.currentIndex
    property int trackid: -1

    color: activePalette.dark
    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: activePalette.light
        }
        GradientStop {
            position:  1.0
            color: activePalette.dark
        }
    }

    ListView {
        id: tracklist
        anchors.margins: 5
        anchors.fill: parent
        smooth: true
        clip:  true

        Database {
            id: database
            table: "tracks"
        }
        ValueSpaceSubscriber {
            id: newid;
            path: "/server/track/id"
            property int trackid: value
            onTrackidChanged: { console.log("TrackId changed"); database.refresh(); }
        }

        Component {
            id: track
            Text {
                text: trackid + " " + name
                color: "white"
                font.pixelSize: 24
                MouseArea {
                    id: mouseArea
                    anchors.fill:  parent
                    onClicked: {
                        root.trackid = trackid
                        tracklist.currentIndex = index
                    }
                }
            }
        }

        model: database
        delegate: track
        highlight: Rectangle { color: "lightsteelblue"; radius: 5; width: parent.width }
        focus: true
    }
}
