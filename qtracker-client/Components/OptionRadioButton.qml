import QtQuick 1.0

OptionTextItem {
    id: root
    property bool ticked: false
    button: true
    buttonsource: ticked? "ticked.svg" : "unticked.svg"
}