import QtQuick 1.0
import "../Components"

import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

Item {
    id: qTracker
    SystemPalette { id: activePalette }

    PageStack {
        id: pageStack
    }

    MainPage {
        id: mainPage
    }

    Component.onCompleted: pageStack.push(mainPage);
}

/*
Rectangle {
    id: root
    width: 360
    height: 360

    Component {
        id: constructor
        Item {
            Text {
                anchors.centerIn: parent
                text: "World"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Qt.quit();
                }
            }
        }
    }

    DynamicItemModel {
        id: itemlist

        Item {
            Text {
                anchors.centerIn: parent
                text: "Hello"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Qt.quit();
                }
            }
        }
    }

    Component.onCompleted: {
        var newitem = constructor.createObject(null)
        itemlist.append(newitem)
    }

    DynamicItemView {
        id: itemview
        model: itemlist
        anchors.fill: parent
    }
}
*/
