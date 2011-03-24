import QtQuick 1.0
import com.nokia.symbian 1.0
//import Qt.labs.components 1.0
import QtMobility.publishsubscribe 1.1

Rectangle {
    id: main
    anchors.fill: parent
    SystemPalette { id: activePalette }
    color: activePalette.dark

    function exitServer() {
        var cmd = { "class": "server", "method": "stop", "args": [] }
        client.sendCommand(cmd);
    }
    function exitClient() {
        Qt.quit();
    }
    function exitClientAndServer() {
        main.exitServer();
        main.exitClient();
    }

    PageStack {
        id: pages
        anchors.fill: parent
    }

    TrackDialog { id: tracks; onCancel: pages.pop(); }
    MapDialog   {
        id: maps;
        onCancel: pages.pop();
        onMapSelected: { view.selectMap(fileName); pages.pop(); }
    }

    Page {
        id: mappage
        signal trackClicked()

        ValueSpaceSubscriber { id: lat;  path: "/server/location/latitude" }
        ValueSpaceSubscriber { id: lon; path: "/server/location/longitude" }

        ToolBar {
            id: toolbar

            Row {
                ToolButton { iconSource: "qrc:/images/options.svg"; onClicked: pages.push(maps);   }
                ToolButton { iconSource: "qrc:/images/flag.svg";    }
                ToolButton { iconSource: "qrc:/images/route.svg";   }
                ToolButton { iconSource: "qrc:/images/hiker.svg";   onClicked: pages.push(tracks); }
                ToolButton { iconSource: "qrc:/images/export.svg";  }

                ToolButton {
                    iconSource: "qrc:/images/exit.svg"
                    property bool longPress: true
                    onClicked:      exitClient()
                    onPressAndHold: exitClientAndServer()
                }
            }
        }

        ValueSpaceSubscriber { id: slat;  path: "/server/location/latitude" }
        ValueSpaceSubscriber { id: slon; path: "/server/location/longitude" }

        ImplicitSizeItem {
            id: statusbar
            anchors.bottom: parent.bottom
            //height: toolbar.height
            implicitHeight: style.current.preferredHeight
            implicitWidth: style.current.preferredWidth

            BorderImage {
                id: background
                anchors.fill: parent
                source: style.current.get("background")
                //border: { left: 20; top: 20; right: 20; bottom: 20 }
                border.left: 20
                border.top: 20
                border.right: 20
                border.bottom: 20
            }

            Style {
                id: style
                styleClass: "toolBar"
            }

            Text {
                id: gpspos
                anchors.leftMargin: 5;
                anchors.topMargin:  5;
                anchors.top: parent.top
                anchors.left: parent.left
                text: slat.value.toString() + " " + slon.value.toString()
                color: "white"
                font.bold: true; font.pixelSize: 15
                style: Text.Raised; styleColor: "white"
            }
            Text {
                id: mappos
                anchors.leftMargin: 5;
                anchors.bottomMargin:  5;
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                text: view.view.position()
                color: "white"
                font.bold: true; font.pixelSize: 15
                style: Text.Raised; styleColor: "white"
            }
        }

        Rectangle {
            anchors.top:    toolbar.bottom
            anchors.bottom: statusbar.top
            MapWrapper {
                id: view
                x: 0
                y: 0
                width: mappage.width
                height: mappage.height-toolbar.height-statusbar.height
            }
        }
    }

    Component.onCompleted: {
        pages.push(mappage)
    }
}
