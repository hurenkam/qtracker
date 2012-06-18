#include <QDateTime>
#include <QVariant>
#include <QString>
#include <QTimer>
#include <QDebug>
#include "timedata.h"

TimeData::TimeData(QObject *parent)
  : QObject(parent)
  , mask(0)
  , current(QDateTime::currentDateTime())
  , elapsed(QDateTime::currentDateTime())
  , monitor(QDateTime::currentDateTime())
{
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(refresh()));
    timer->start(1000);
}

void TimeData::registerApi(XmlRpcServer* srv)
{
    srv->registerSlot( this, SLOT( data() ),                                  "/RPC2/time/" );
    srv->registerSlot( this, SLOT( reset() ),                                 "/RPC2/time/" );
}

void TimeData::refresh()
{
    //qDebug() << "TimeData::refresh()";
    current = QDateTime::currentDateTime();
}

QVariant TimeData::data()
{
    QMap<QString, QVariant> r;
    r["mask"]    = mask;
    r["current"] = current;
    r["elapsed"] = elapsed;
    r["monitor"] = monitor;
    return r;
}
