import QtQuick 1.0

Item {}

/* Obsolete

Page {
    id: root
    signal mapSelected(string fileName)
    function toggleLocal() {
    }
    function addRefpoint() {
    }

    ToolBar {
        id: toolbar

        tools: ToolBarLayout {
            hasRightButton: true
            ToolButton { id: backbutton;    source: "qrc:/images/import.svg";  onClicked: pageStack.pop();                   }
            ToolButton { id: allbutton;     source: "qrc:/images/options.svg"; onClicked: root.toggleLocal();                }
            ToolButton { id: addbutton;     source: "qrc:/images/zoom-in.svg"; onClicked: root.addRefpoint();                }
            ToolButton {
                id: confirmbutton;
                source: "qrc:/images/visible.svg";
                onClicked: {
                    root.mapSelected(maplist.mapname);
                    pageStack.pop();
                }
            }
        }
    }

    MapList {
        id: maplist
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        width: root.width
        index: -1
    }
}
*/
