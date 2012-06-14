#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

#if !defined(Q_OS_ANDROID)
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("main/Main.qml"));
    viewer->resize(QSize(540,960)); // HTC Evo 3D
    //viewer->resize(QSize(480,800)); // Galaxy S2
    viewer->showExpanded();
#else
    viewer->setSource(QUrl("qrc:///main/Main.qml"));
    viewer->showFullScreen();
#endif

    return app->exec();
}
