import QtQuick 1.0
import "../Components"

import QtQuick 1.0
import QtMobility.publishsubscribe 1.1

Item {
    id: qTracker
    SystemPalette { id: activePalette }

    PageStack {
        id: pageStack
        platform: client.platform
    }

    MainPage {
        id: mainPage
    }

    Component.onCompleted: pageStack.push(mainPage);
}
