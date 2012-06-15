#include <QDebug>
#include "compassserver.h"

//#define ENABLE_DEBUG
#include "helpers.h"

CompassServer::CompassServer(QObject *parent)
    : QObject(parent)
    , p(0)
    , c(0)
    , r(0)
{
    ENTER("")
    EXIT("")
}

CompassServer::~CompassServer()
{
    ENTER("")
    if (p) stop();
    EXIT("")
}

void CompassServer::onReadingChanged()
{
    ENTER("")

    QVariant v;
    double azimuth      = r->azimuth();
    double calibration  = r->calibrationLevel();
    v = azimuth;
    p->setValue("azimuth",v);
    v = calibration;
    p->setValue("calibration",v);

    EXIT("")
}

void CompassServer::start()
{
    ENTER("")

    if (c || r || p) return;
    p = new QValueSpacePublisher("/server/compass");

    double d = 0.0;
    p->setValue("azimuth",d);
    p->setValue("calibration",d);

    c = new QCompass();
    if (c)
    {
        c->setDataRate(4);
        if (c->start())
        {
            r = c->reading();
            if (r)
            {
                onReadingChanged();
                connect(c, SIGNAL(readingChanged()), this, SLOT(onReadingChanged()));
            }
        }
    }
    emit started();

    EXIT("")
}

void CompassServer::stop()
{
    ENTER("")

    if (c) c->stop();

    delete r;
    delete c;
    delete p;
    c = 0;
    r = 0;
    p = 0;
    emit stopped();

    EXIT("")
}
