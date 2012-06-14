import QtQuick 1.0

OptionTextItem {
    id: root
    property bool ticked: false
    button: true
    buttonsource: ticked? "ticked.png" : "unticked.png"
}
