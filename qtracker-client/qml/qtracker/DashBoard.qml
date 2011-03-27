import QtQuick 1.0

Item {
    id: root
    property bool hide: false
    height: hide? bgimage.height : (landscape? 70*h: 200*w)
    //visible: hide? false : true
    //Behavior on y { NumberAnimation { easing.type: Easing.InOutQuart; duration: 400 } }

    Image {
        id: bgimage
        source: landscape? "qrc:/images/dash-bg-landscape.ico": "qrc:/images/dash-bg-portrait.ico"
        anchors.bottom: parent.bottom
        height: sourceSize.height*w
        width: sourceSize.width*w
    }
    Clock {
        id:     clock
        x:      landscape? 5*w: 0*w
        y:      landscape? parent.height-80*w: parent.height-200*w
        width:  80*w
        height: 80*w
    }
    Compass {
        id:     compass
        x:      landscape?               440*w:                80*w
        y:      landscape? parent.height-200*w: parent.height-275*w
        width:  198*w
        height: 198*w
    }
    Satellites {
        id:     sats
        x:      landscape? 365*w: 280*w
        y:      landscape? parent.height-85*w: parent.height-200*w
        width:  80*w
        height: 80*w
    }
    Speedometer {
        id:     speed
        x:      0
        y:      landscape? parent.height-205*w: parent.height-120*w
        width:  120*w
        height: 120*w
    }
    Altimeter {
        id:     altimeter
        x:      landscape? 90*w: 240*w
        y:      parent.height-120*w
        width:  120*w
        height: 120*w
    }

    Image {
        source: landscape? "qrc:/images/dash-fg-landscape.ico": "qrc:/images/dash-fg-portrait.ico"
        anchors.bottom: parent.bottom
        height: sourceSize.height*w
        width: sourceSize.width*w
    }
}
