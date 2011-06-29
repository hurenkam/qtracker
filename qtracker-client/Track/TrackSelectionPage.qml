import QtQuick 1.0
import QtMobility.publishsubscribe 1.1
import "../Components"
import "../Map"
import "../Main"

MainOptionPage {
    id: root
    title: "Track List"
    options: trkoptions
    //property MapView mapview: null
    property int catid: -1
    property int tripid: -1

    TrackRecordingPage {
        id: trkrecord;
        onTrackSaved: {
            var trackid = lst.saveTrack(index,name,interval)
            trkrecord.startTrack(trackid,interval)
        }
    }

    ValueSpaceSubscriber  {
        id: currenttrack;
        path: "/server/track/id"
        property int trackid: value
    }

    VisualItemModel {
        id: trkoptions

        TrackList {
            id: lst
            //title: "Route List"
            onRecordTrack: { trkrecord.index = index; pageStack.push(trkrecord);    }
            onEditTrack:   { trkrecord.index = index; pageStack.push(trkrecord);    }
            onStopTrack:   { trkrecord.stopTrack(currenttrack.trackid); }
        }
    }
}
