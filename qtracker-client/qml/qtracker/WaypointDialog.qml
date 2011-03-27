import QtQuick 1.0

Page {
    id: root

    ToolBar {
        id: toolbar

        tools: ToolBarLayout {
            ToolButton { id: backbutton; source: "qrc:/images/import.svg"; onClicked: pageStack.pop(); }
        }
    }
}
