#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QUrl>
#include <QPixmap>
#include <QSplashScreen>
#include <QWaitCondition>
#include <QDebug>
#include "qmlapplicationviewer.h"

#include "folderlistmodel.h"
#include "deviceinfomodel.h"
#include "mapview.h"
#include "client.h"
#include "tablemodel.h"

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
#if   defined(Q_OS_SYMBIAN)
    file.setFileName("e:/data/mapviewer-debug.txt");
#elif defined(Q_WS_MAEMO_5)
    file.setFileName("mapviewer-debug.txt");
#elif defined(Q_WS_SIMULATOR)
    file.setFileName("c:/data/mapviewer-debug.txt");
#else
    file.setFileName("mapviewer-debug.txt");
#endif
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
    qmlRegisterType<DeviceInfoModel>("QmlTrackerExtensions",1,0,"DeviceInfoModel");
    qmlRegisterType<MapView>("QmlTrackerExtensions",1,0,"MapView2");
    qmlRegisterType<TableModel>("QmlTrackerExtensions",1,0,"TableModel");
}

int main(int argc, char *argv[])
{
    debugOpen();
    registerTypes();
    QApplication app(argc, argv);

    Client client;
    client.startServer();

    qDebug() << "main(): splash.show()";
    QPixmap pixmap(":/images/splash.svg");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("qTracker v" VERSION,Qt::AlignLeft,Qt::white);

    qDebug() << "main(): app.processEvents()";
    app.processEvents();

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setSource(QUrl("qrc:///qml/qTracker.qml"));
    viewer.rootContext()->setContextProperty("client",&client);
    splash.finish(&viewer);
    viewer.showFullScreen();
    //viewer.showMaximized();

    qDebug() << "main(): app.exec()";
    int result = app.exec();
    debugClose();
    return result;
}
