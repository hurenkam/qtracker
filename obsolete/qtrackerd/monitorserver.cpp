#include <QDebug>
#include <QVariantMap>
#include "monitorserver.h"

//#define ENABLE_DEBUG
#include "helpers.h"
MonitorServer::MonitorServer(QObject *parent)
    : QObject(parent)
    , p(0)
{
    ENTER("")
    EXIT("")
}

MonitorServer::~MonitorServer()
{
    ENTER("")
    if (p) stop();
    EXIT("")
}

void MonitorServer::start()
{
    ENTER("")

    if (p) return;
    p = new QValueSpacePublisher("/server/monitor");

    double d = 0.0;
    int i = 0;
    QVariantMap vt;
    vt["hour"] = i;
    vt["minute"] = i;
    vt["second"] = i;

    p->setValue("bearing",d);
    p->setValue("time",vt);
    p->setValue("distance",d);

    emit started();

    EXIT("")
}

void MonitorServer::stop()
{
    ENTER("")

    delete p;
    p = 0;
    emit stopped();

    EXIT("")
}

void MonitorServer::onPositionChanged(double lat, double lon, double alt)
{
    ENTER(lat << "," << lon << "," << alt)
    EXIT("")
}

void MonitorServer::onDistanceChanged(double d)
{
    ENTER(d)
    EXIT("")
}

void MonitorServer::onTimeChanged(const QTime& t)
{
    ENTER(t)
    EXIT("")
}

void MonitorServer::onSpeedChanged(double s)
{
    ENTER(s)
    EXIT("")
}

void MonitorServer::onHeadingChanged(double h)
{
    ENTER(h)
    EXIT("")
}
