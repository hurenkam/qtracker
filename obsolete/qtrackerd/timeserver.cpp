#include <QDebug>
#include <QVariant>
#include "timeserver.h"

//#define ENABLE_DEBUG
#include "helpers.h"

TimeServer::TimeServer(QObject *parent)
    : CommandHandler(parent)
    , p(0)
    , t(0)
{
    ENTER("")
    methods << "reset";
    EXIT("")
}

void TimeServer::onCommand(int method, QVariantList args)
{
    ENTER("")

    switch (method)
    {
        case 0:
            reset();
            break;
    }

    EXIT("")
}

TimeServer::~TimeServer()
{
    ENTER("")
    if (p) stop();
    EXIT("")
}

void TimeServer::onTimeChanged()
{
    ENTER("")

    QTime current = QDateTime::currentDateTime().time();
    QTime trip = QTime(0,0,0).addSecs(_starttime.secsTo(current));

    QVariantMap vc;
    vc["hour"]   = current.hour();
    vc["minute"] = current.minute();
    vc["second"] = current.second();
    p->setValue("current",vc);

    QVariantMap vt;
    vt["hour"]   = trip.hour();
    vt["minute"] = trip.minute();
    vt["second"] = trip.second();
    p->setValue("trip",vt);

    emit currentTimeChanged(current);
    emit tripTimeChanged(trip);

    EXIT("")
}

void TimeServer::reset()
{
    ENTER("")

    _starttime = QDateTime::currentDateTime().time();
    onTimeChanged();

    EXIT("")
}

void TimeServer::start()
{
    ENTER("")

    if (t || p) return;

    p = new QValueSpacePublisher("/server/time");
    t = new QTimer();
    t->start(1000);

    _starttime = QDateTime::currentDateTime().time();
    onTimeChanged();

    connect(t, SIGNAL(timeout()), this, SLOT(onTimeChanged()));
    emit started();

    EXIT("")
}

void TimeServer::stop()
{
    ENTER("")

    t->stop();

    delete p;
    delete t;
    p = 0;
    t = 0;
    emit stopped();

    EXIT("")
}
