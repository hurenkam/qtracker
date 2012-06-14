import QtQuick 1.0

OptionItem {
    property alias text: txt.text
    height: txt.height

    Text {
        id: txt
        color: "black"
        //font.bold: true
        font.pointSize: 8
        text: ""
    }
}
