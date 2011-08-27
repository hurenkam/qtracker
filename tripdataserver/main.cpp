#include <QtCore/QCoreApplication>
#include "tripdataserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TripDataServer* svr = new TripDataServer();
    a.connect(svr,SIGNAL(quit()),SLOT(quit()));
    int result = a.exec();
    delete svr;
    return result;
}
