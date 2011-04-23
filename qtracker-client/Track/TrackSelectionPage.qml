import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"

OptionPage {
    id: root
    title: "Track List"
    options: trkoptions

    TrackRecordingPage { id: trkrecord }

    VisualItemModel {
        id: trkoptions

        TrackList {
            id: lst
            //title: "Route List"
            onRecordTrack: { trkrecord.index = index; pageStack.push(trkrecord); }
        }
    }
}
