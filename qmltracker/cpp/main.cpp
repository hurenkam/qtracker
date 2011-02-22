#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QUrl>
#include <QPixmap>
#include <QSplashScreen>
#include <QWaitCondition>
#include "qmlapplicationviewer.h"

#include "folderlistmodel.h"
#include "altitudemodel.h"
#include "speedmodel.h"
#include "clockmodel.h"
#include "compassmodel.h"
#include "monitormodel.h"
#include "positionmodel.h"
#include "deviceinfomodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<QDeclarativeFolderListModel>("QmlTrackerExtensions",1,0,"FolderListModel");
    qmlRegisterType<AltitudeModel>("QmlTrackerExtensions",1,0,"AltitudeModel");
    qmlRegisterType<SpeedModel>("QmlTrackerExtensions",1,0,"SpeedModel");
    qmlRegisterType<ClockModel>("QmlTrackerExtensions",1,0,"ClockModel");
    qmlRegisterType<CompassModel>("QmlTrackerExtensions",1,0,"CompassModel");
    qmlRegisterType<MonitorModel>("QmlTrackerExtensions",1,0,"MonitorModel");
    qmlRegisterType<PositionModel>("QmlTrackerExtensions",1,0,"PositionModel");
    qmlRegisterType<DeviceInfoModel>("QmlTrackerExtensions",1,0,"DeviceInfoModel");

    QPixmap pixmap(":/images/splash.svg");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("qTracker v" VERSION,Qt::AlignLeft,Qt::white);
    app.processEvents();

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setSource(QUrl("qrc:///qml/main.qml"));
    splash.hide();
    viewer.showFullScreen();
    return app.exec();
}

