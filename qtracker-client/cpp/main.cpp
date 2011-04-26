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

#define ENABLE_DEBUG
#include "helpers.h"

static QFile file;
static QTextStream out(&file);
static bool fileopen = false;

void debugOpen();
void debugClose();

void debugOutput(QtMsgType type, const char *msg)
{
        //debugOpen();
        out << QDateTime::currentDateTime().toString() << "    " << msg << "\n";
        file.flush();
        //debugClose();
}

void debugOpen()
{
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
    file.setFileName("e:/data/qtracker-debug.txt");
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
    QString dataPath = "e:\\data\\qTracker";
    LOG("engine.offlineStoragePath() old: " << viewer.engine()->offlineStoragePath())
    viewer.engine()->setOfflineStoragePath(dataPath);
    LOG("engine.offlineStoragePath() new: " << viewer.engine()->offlineStoragePath())

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    //viewer.setSource(QUrl("qrc:///qml/qTracker.qml"));
    viewer.setMainQmlFile(QLatin1String("Main/main.qml"));
    viewer.rootContext()->setContextProperty("client",&client);
    splash.finish(&viewer);
    viewer.showFullScreen();

    LOG("main(): app.exec()")
    int result = app.exec();
    debugClose();
    return result;
}
