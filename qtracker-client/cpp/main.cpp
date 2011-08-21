#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QUrl>
#include <QPixmap>
#include <QSplashScreen>
#include <QWaitCondition>
#include <QDebug>
#include <QDateTime>
#include <QDeclarativeEngine>
#include "qmlapplicationviewer.h"

#include "folderlistmodel.h"
#include "deviceinfomodel.h"
#include "mapview.h"
#include "gpxfile.h"
#include "qmldatabase.h"

#if defined(Q_OS_SYMBIAN)
#include "client.h"
#elif defined (Q_WS_SIMULATOR)
#include "client.h"
#elif defined(Q_OS_ANDROID)
#include "../tripdataserver/tripserverinterface.h"
#endif

//#define ENABLE_DEBUG
#include "helpers.h"


static QFile file;
static QTextStream out(&file);
static bool fileopen = false;

void debugOpen();
void debugClose();

QString getStoragePath()
{
#if   defined(Q_OS_SYMBIAN)
    return "e:/data/qTracker";
#elif defined(Q_WS_MAEMO_5)
    return ".";
#elif defined(Q_WS_SIMULATOR)
    return "c:/data/qTracker";
#elif defined(Q_OS_ANDROID)
    return "/mnt/sdcard/qtracker";
#else
    return ".";
#endif
}

void debugOutput(QtMsgType type, const char *msg)
{
        //debugOpen();
        out << QDateTime::currentDateTime().toString() << "    " << msg << "\n";
        file.flush();
        //debugClose();
}

void debugOpen()
{
    file.setFileName(getStoragePath() + "/qtracker-debug.txt");

    if (file.exists())
        fileopen = file.open(QIODevice::Append | QIODevice::Text);
    else
        fileopen = false;

    if (fileopen)
        qInstallMsgHandler(debugOutput);

    LOG("")
    LOG("")
    LOG("====================================================")
    LOG("Starting debug log for qtracker " << VERSION)
    LOG("----------------------------------------------------")
    LOG("")
}

void debugClose()
{
    LOG("")
    LOG("----------------------------------------------------")
    LOG("Closing debug log for qtracker " << VERSION)
    LOG("====================================================")
    LOG("")
    LOG("")

    if (fileopen)
        file.close();
}

void registerTypes()
{
    LOG("Enter: registerTypes()")
    qmlRegisterType<QDeclarativeFolderListModel>("QmlTrackerExtensions",1,0,"FolderListModel");
    qmlRegisterType<DeviceInfoModel>("QmlTrackerExtensions",1,0,"DeviceInfoModel");
    qmlRegisterType<MapView>("QmlTrackerExtensions",1,0,"MapView2");
    qmlRegisterType<GpxFile>("QmlTrackerExtensions",1,0,"GpxFile");
    qmlRegisterType<qmlDatabase>("QmlTrackerExtensions",1,0,"TDatabase");
    qmlRegisterType<qmlCategory>("QmlTrackerExtensions",1,0,"TCategory");
    qmlRegisterType<qmlMap>("QmlTrackerExtensions",1,0,"TMap");
    qmlRegisterType<qmlTrip>("QmlTrackerExtensions",1,0,"TTrip");
    qmlRegisterType<qmlWaypoint>("QmlTrackerExtensions",1,0,"TWaypoint");
    qmlRegisterType<qmlRoute>("QmlTrackerExtensions",1,0,"TRoute");
    qmlRegisterType<qmlTrack>("QmlTrackerExtensions",1,0,"TTrack");
    qmlRegisterType<qmlRefpoint>("QmlTrackerExtensions",1,0,"TRefpoint");
#if defined(Q_OS_ANDROID)
    qmlRegisterType<TripServerInterface>("QmlTrackerExtensions",1,0,"TripServer");
#endif
    LOG("Exit: registerTypes()")
}

int main(int argc, char *argv[])
{
    debugOpen();
    registerTypes();
    QApplication app(argc, argv);

#if !defined(Q_OS_ANDROID)
    Client client;
    client.startServer();

    LOG("main(): splash.show()")
    QPixmap pixmap(QLatin1String("Images/splash.svg"));
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("qTracker v" VERSION,Qt::AlignLeft,Qt::white);
#endif

    LOG("main(): app.processEvents()")
    app.processEvents();

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
#if defined(Q_OS_ANDROID)
    viewer.setSource(QUrl("qrc:///Main/main.qml"));
    viewer.showFullScreen();
#else
    QString dataPath = getStoragePath();
    LOG("engine.offlineStoragePath() old: " << viewer.engine()->offlineStoragePath())
    viewer.engine()->setOfflineStoragePath(dataPath);
    LOG("engine.offlineStoragePath() new: " << viewer.engine()->offlineStoragePath())
    viewer.rootContext()->setContextProperty("client",&client);
    viewer.setMainQmlFile(QLatin1String("Main/main.qml"));
    splash.finish(&viewer);
    viewer.showFullScreen();
#endif

    LOG("main(): app.exec()")
    int result = app.exec();
    debugClose();
    return result;
}
