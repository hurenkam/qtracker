#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QDeclarativeEngine>
#include "qmlapplicationviewer.h"
#include "qmldatabase.h"

void registerTypes()
{
//    qmlRegisterType<QDeclarativeFolderListModel>("QmlTrackerExtensions",1,0,"FolderListModel");
//    qmlRegisterType<DeviceInfoModel>("QmlTrackerExtensions",1,0,"DeviceInfoModel");
//    qmlRegisterType<MapView>("QmlTrackerExtensions",1,0,"MapView2");
//    qmlRegisterType<GpxFile>("QmlTrackerExtensions",1,0,"GpxFile");
    qmlRegisterType<qmlDatabase>("QmlTrackerExtensions",1,0,"TDatabase");
    qmlRegisterType<qmlCategory>("QmlTrackerExtensions",1,0,"TCategory");
    qmlRegisterType<qmlMap>("QmlTrackerExtensions",1,0,"TMap");
    qmlRegisterType<qmlTrip>("QmlTrackerExtensions",1,0,"TTrip");
    qmlRegisterType<qmlWaypoint>("QmlTrackerExtensions",1,0,"TWaypoint");
    qmlRegisterType<qmlRoute>("QmlTrackerExtensions",1,0,"TRoute");
    qmlRegisterType<qmlTrack>("QmlTrackerExtensions",1,0,"TTrack");
    qmlRegisterType<qmlRefpoint>("QmlTrackerExtensions",1,0,"TRefpoint");
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    registerTypes();
#if defined(Q_OS_ANDROID) || defined(Q_WS_SIMULATOR) || defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
    viewer->setSource(QUrl("qrc:///main/Main.qml"));
    viewer->showFullScreen();
#else
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("main/Main.qml"));
    viewer->resize(QSize(540,960)); // HTC Evo 3D
    //viewer->resize(QSize(480,800)); // Galaxy S2
    viewer->showExpanded();
#endif

    return app->exec();
}
