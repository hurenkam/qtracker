import QtQuick 1.0
import "../Components"

Item {
    id: qTracker
    Item {
        id: activePalette
        property color light: "#505050"
        property color dark: "#101010"
    }

    PageStack {
        id: pageStack
    }

    MainPage {
        id: mainPage
    }

    Component.onCompleted: pageStack.push(mainPage);
}
