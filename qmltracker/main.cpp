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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DataMonitor data;

    qmlRegisterType<QDeclarativeFolderListModel>("QmlTrackerExtensions",1,0,"FolderListModel");

    QPixmap pixmap(":/qml/splash.svg");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("qTracker v" VERSION,Qt::AlignLeft);
    app.processEvents();

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.rootContext()->setContextProperty("model",&data);
    viewer.setSource(QUrl("qrc:///qml/main.qml"));
    splash.hide();
    viewer.showFullScreen();
    return app.exec();
}
