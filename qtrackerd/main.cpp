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
    //debugOpen();

    out << msg << "\n";
    file.flush();

    //debugClose();
}

void debugOpen()
{
    file.setFileName("e:/data/server-debug.txt");
    if (file.exists())
        fileopen = file.open(QIODevice::Append | QIODevice::Text);
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


int main(int argc, char *argv[])
{
    //qInstallMsgHandler(debugOutput);
    debugOpen();
    LOG("Enter: main()")

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
