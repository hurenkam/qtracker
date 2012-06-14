import QtQuick 1.0
import "../components"
import "../model"

Rectangle {
    id: qTracker
    width: 480
    height: 800

    Item {
        id: activePalette
        property color light: "#505050"
        property color dark: "#101010"
    }

    PageStack {
        id: pageStack
        onPopLast: Qt.quit()
    }

    MainPage {
        id: mainPage
    }

    Component.onCompleted: pageStack.push(mainPage);
}
