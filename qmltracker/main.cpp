#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QUrl>
#include <QPixmap>
#include <QSplashScreen>
#include <QWaitCondition>
#include "datamonitor.h"
#include "qmlapplicationviewer.h"

//#include <QtDeclarative/qdeclarativeextensionplugin.h>
//#include <QtDeclarative/qdeclarative.h>
#include "cpp/folderlistmodel.h"
#include "cpp/altitudemodel.h"
#include "cpp/speedmodel.h"
#include "cpp/clockmodel.h"
#include "cpp/compassmodel.h"
#include "cpp/monitormodel.h"
#include "cpp/positionmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DataMonitor& data = DataMonitor::Instance();

    qmlRegisterType<QDeclarativeFolderListModel>("QmlTrackerExtensions",1,0,"FolderListModel");
    qmlRegisterType<AltitudeModel>("QmlTrackerExtensions",1,0,"AltitudeModel");
    qmlRegisterType<SpeedModel>("QmlTrackerExtensions",1,0,"SpeedModel");
    qmlRegisterType<ClockModel>("QmlTrackerExtensions",1,0,"ClockModel");
    qmlRegisterType<CompassModel>("QmlTrackerExtensions",1,0,"CompassModel");
    qmlRegisterType<MonitorModel>("QmlTrackerExtensions",1,0,"MonitorModel");
    qmlRegisterType<PositionModel>("QmlTrackerExtensions",1,0,"PositionModel");

    QPixmap pixmap(":/images/splash.svg");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("qTracker v" VERSION,Qt::AlignLeft,Qt::white);
    app.processEvents();

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.rootContext()->setContextProperty("model",&data);
    viewer.setSource(QUrl("qrc:///qml/main.qml"));
    splash.hide();
    viewer.showFullScreen();
    return app.exec();
}
