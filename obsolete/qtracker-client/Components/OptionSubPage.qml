import QtQuick 1.0

Item {
    id: root
    x:  0
    y:  0
    width:  parent.width
    height: parent.height-y

    //property alias backbutton: hdr.leftButtonVisible
    //property alias confirmbutton: hdr.rightButtonVisible
    property alias leftbutton: hdr.leftButtonVisible
    property alias leftbuttonsrc: hdr.leftButtonSource
    property alias rightbutton: hdr.rightButtonVisible
    property alias rightbuttonsrc: hdr.rightButtonSource
    property alias title: hdr.text

    signal cancel()
    signal confirm()
    signal leftClicked()
    signal rightClicked()

    Image {
        id: image
        anchors.fill: parent
        visible: (source != "")
        source: "../Images/options-bg.png"
        fillMode: Image.Tile
    }

    Item {
        id: title
        x: 0;
        y: 0;
        width: root.width;
        height: visible? 60 : 0;
        z: 5;

        OptionHeader {
            id: hdr
            anchors.fill: parent
            //visible: root.tools? false: true
            text: "Options"
            leftButtonVisible: true
            onLeftClicked: root.leftClicked();
            leftButtonSource: "../Images/down-plain.png"
            rightButtonVisible: true
            rightButtonSource: "../Images/add-plain.png"
            onRightClicked: root.rightClicked();
            buttonheight: title.height
        }
    }
}
