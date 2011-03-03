#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QUrl>
#include <QPixmap>
#include <QSplashScreen>
#include <QWaitCondition>
#include <QDebug>
#include "qmlapplicationviewer.h"

#include "folderlistmodel.h"
#include "altitudemodel.h"
#include "speedmodel.h"
#include "clockmodel.h"
#include "compassmodel.h"
#include "monitormodel.h"
#include "positionmodel.h"
#include "deviceinfomodel.h"
#include "satellitemodel.h"
#include "mapview.h"

static QFile file;
static QTextStream out(&file);
static bool fileopen = false;

void debugOutput(QtMsgType type, const char *msg)
{
        out << msg << "\n";
        file.flush();
}

void debugOpen()
{
        file.setFileName("e:/data/qmltracker-debug.txt");
        if (file.exists())
            fileopen = file.open(QIODevice::Append | QIODevice::Truncate | QIODevice::Text);
        else
                fileopen = false;

        if (fileopen)
                qInstallMsgHandler(debugOutput);
}

void debugClose()
{
    if (fileopen)
        file.close();
}

void registerTypes()
{
    qmlRegisterType<QDeclarativeFolderListModel>("QmlTrackerExtensions",1,0,"FolderListModel");
    qmlRegisterType<AltitudeModel>("QmlTrackerExtensions",1,0,"AltitudeModel");
    qmlRegisterType<SpeedModel>("QmlTrackerExtensions",1,0,"SpeedModel");
    qmlRegisterType<ClockModel>("QmlTrackerExtensions",1,0,"ClockModel");
    qmlRegisterType<CompassModel>("QmlTrackerExtensions",1,0,"CompassModel");
    qmlRegisterType<MonitorModel>("QmlTrackerExtensions",1,0,"MonitorModel");
    qmlRegisterType<PositionModel>("QmlTrackerExtensions",1,0,"PositionModel");
    qmlRegisterType<DeviceInfoModel>("QmlTrackerExtensions",1,0,"DeviceInfoModel");
    qmlRegisterType<SatelliteInfo>("QmlTrackerExtensions",1,0,"SatelliteInfo");
    qmlRegisterType<SatelliteList>("QmlTrackerExtensions",1,0,"SatelliteList");
    qmlRegisterType<MapView>("QmlTrackerExtensions",1,0,"MapView2");
}

int main(int argc, char *argv[])
{
    debugOpen();
    registerTypes();
    QApplication app(argc, argv);

    qDebug() << "main(): splash.show()";
    QPixmap pixmap(":/images/splash.svg");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("qTracker v" VERSION,Qt::AlignLeft,Qt::white);

    qDebug() << "main(): app.processEvents()";
    app.processEvents();

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setSource(QUrl("qrc:///qml/main.qml"));
    splash.finish(&viewer);
    viewer.showFullScreen();

    qDebug() << "main(): app.exec()";
    int result = app.exec();
    debugClose();
    return result;
}
