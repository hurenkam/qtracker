#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QUrl>
#include "datamonitor.h"
#include "layout.h"
#include "map.h"
#include "qmlapplicationviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DataMonitor data;

    //qmlRegisterType<Widget>("QmlTrackerModel",1,0,"Widget");
    //qmlRegisterType<Layout>("QmlTrackerModel",1,0,"Layout");
    qmlRegisterType<Map>("QmlTrackerModel",1,0,"Map");
    qmlRegisterType<Position>("QmlTrackerModel",1,0,"Position");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.rootContext()->setContextProperty("model",&data);
    viewer.setSource(QUrl("qrc:///qml/main.qml"));
    viewer.showFullScreen();

    return app.exec();
}
