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
#include "client.h"
#include "tablemodel.h"
#include "gpxfile.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
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
/*
#if   defined(Q_OS_SYMBIAN)
    file.setFileName("e:/data/qtracker-debug.txt");
#elif defined(Q_WS_MAEMO_5)
    file.setFileName("qtracker-debug.txt");
#elif defined(Q_WS_SIMULATOR)
    file.setFileName("c:/data/qtracker-debug.txt");
#else
    file.setFileName("qtracker-debug.txt");
#endif
*/
    //file.setFileName("e:/data/qtracker-debug.txt");
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
    qmlRegisterType<TableModel>("QmlTrackerExtensions",1,0,"TableModel");
    qmlRegisterType<GpxFile>("QmlTrackerExtensions",1,0,"GpxFile");
    qmlRegisterType<qmlDatabase>("QmlTrackerExtensions",1,0,"TDatabase");
    qmlRegisterType<qmlCategory>("QmlTrackerExtensions",1,0,"TCategory");
    qmlRegisterType<qmlMap>("QmlTrackerExtensions",1,0,"TMap");
    qmlRegisterType<qmlTrip>("QmlTrackerExtensions",1,0,"TTrip");
    qmlRegisterType<qmlWaypoint>("QmlTrackerExtensions",1,0,"TWaypoint");
    qmlRegisterType<qmlRoute>("QmlTrackerExtensions",1,0,"TRoute");
    qmlRegisterType<qmlTrack>("QmlTrackerExtensions",1,0,"TTrack");
    qmlRegisterType<qmlRefpoint>("QmlTrackerExtensions",1,0,"TRefpoint");
    LOG("Exit: registerTypes()")
}

int main(int argc, char *argv[])
{
    debugOpen();
    registerTypes();
    QApplication app(argc, argv);

    Client client;
    client.startServer();

    LOG("main(): splash.show()")
    QPixmap pixmap(QLatin1String("Main/splash.svg"));
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("qTracker v" VERSION,Qt::AlignLeft,Qt::white);

    LOG("main(): app.processEvents()")
    app.processEvents();

    QmlApplicationViewer viewer;
    QString dataPath = getStoragePath();
    LOG("engine.offlineStoragePath() old: " << viewer.engine()->offlineStoragePath())
    viewer.engine()->setOfflineStoragePath(dataPath);
    LOG("engine.offlineStoragePath() new: " << viewer.engine()->offlineStoragePath())

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    //viewer.setSource(QUrl("qrc:///qml/qTracker.qml"));
    viewer.rootContext()->setContextProperty("client",&client);
    viewer.setMainQmlFile(QLatin1String("Main/main.qml"));
    splash.finish(&viewer);
    viewer.showFullScreen();

    LOG("main(): app.exec()")
    int result = app.exec();
    debugClose();
    return result;
}
