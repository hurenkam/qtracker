#include <QtCore/QCoreApplication>
#include <QFile>
#include <QTextStream>
#include "server.h"

#define ENABLE_DEBUG
#include "helpers.h"

static QFile file;
static QTextStream out(&file);
static bool fileopen = false;

void debugOpen();
void debugClose();

void debugOutput(QtMsgType type, const char *msg)
{
        out << QDateTime::currentDateTime().toString() << "    " << msg << "\n";
        file.flush();
}

void debugOpen()
{
//#if   defined(Q_OS_SYMBIAN)
    file.setFileName("e:/data/qtrackerd-debug.txt");
//#elif defined(Q_WS_MAEMO_5)
//    file.setFileName("qtrackerd-debug.txt");
//#elif defined(Q_WS_SIMULATOR)
//    file.setFileName("c:/data/qtrackerd-debug.txt");
//#else
//    file.setFileName("qtrackerd-debug.txt");
//#endif
    if (file.exists())
        fileopen = file.open(QIODevice::Append | QIODevice::Text);
    else
        fileopen = false;

    if (fileopen)
        qInstallMsgHandler(debugOutput);

    LOG("")
    LOG("")
    LOG("====================================================")
    LOG("Starting debug log for qtrackerd " << VERSION)
    LOG("----------------------------------------------------")
    LOG("")
}

void debugClose()
{
    LOG("")
    LOG("----------------------------------------------------")
    LOG("Closing debug log for qtrackerd " << VERSION)
    LOG("====================================================")
    LOG("")
    LOG("")

    if (fileopen)
        file.close();
}





int main(int argc, char *argv[])
{
    //qInstallMsgHandler(debugOutput);
    debugOpen();
    LOG("Enter: main() ")

    QValueSpace::initValueSpaceServer();
    QCoreApplication a(argc, argv);
    Server server(&a);

    QObject::connect(&server, SIGNAL(stopped()), &a, SLOT(quit()));

    server.start();
    int result = a.exec();

    LOG("Exit: main()")
    debugClose();
    return result;
}
