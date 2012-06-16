#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"
#include "daemon.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

#if defined(Q_OS_ANDROID) || defined(Q_WS_SIMULATOR) || defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
    viewer->setSource(QUrl("main/Main.qml"));
    viewer->showFullScreen();
#else
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("main/Main.qml"));
    viewer->resize(QSize(540,960)); // HTC Evo 3D
    //viewer->resize(QSize(480,800)); // Galaxy S2
    viewer->showExpanded();
#endif

    QString address= "127.0.0.1";
    quint16 port= 8280;
    Daemon  daemon( address, port );
    viewer->rootContext()->setContextProperty("daemon", &daemon);

    return app->exec();
}
