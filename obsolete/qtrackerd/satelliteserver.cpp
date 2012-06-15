#include <QVariant>
#include <QDebug>
#include "satelliteserver.h"

//#define ENABLE_DEBUG
#include "helpers.h"

SatelliteServer::SatelliteServer(QObject *parent)
    : QObject(parent)
    , p(0)
    , s(0)
{
    ENTER("")
    EXIT("")
}

SatelliteServer::~SatelliteServer()
{
    ENTER("")
    if (s) stop();
    EXIT("")
}

void SatelliteServer::onInViewChanged(const QList<QGeoSatelliteInfo>& info)
{
    ENTER("")

    _inview.clear();
    for (int i=0; i<info.count(); ++i)
    {
        QVariantMap m;
        m["strength"]  = info[i].signalStrength();
        m["azimuth"]   = info[i].hasAttribute(QGeoSatelliteInfo::Azimuth)?   info[i].attribute(QGeoSatelliteInfo::Azimuth)   : 0;
        m["elevation"] = info[i].hasAttribute(QGeoSatelliteInfo::Elevation)? info[i].attribute(QGeoSatelliteInfo::Elevation) : 0;
        _inview.append(m);
    }
    p->setValue("inview",_inview);

    EXIT("")
}

void SatelliteServer::onInUseChanged(const QList<QGeoSatelliteInfo>& info)
{
    ENTER("")

    _inuse.clear();
    for (int i=0; i<info.count(); ++i)
    {
        QVariantMap m;
        m["strength"]  = info[i].signalStrength();
        m["azimuth"]   = info[i].hasAttribute(QGeoSatelliteInfo::Azimuth)?   info[i].attribute(QGeoSatelliteInfo::Azimuth)   : 0;
        m["elevation"] = info[i].hasAttribute(QGeoSatelliteInfo::Elevation)? info[i].attribute(QGeoSatelliteInfo::Elevation) : 0;
        _inuse.append(m);
    }
    p->setValue("inuse",_inuse);

    EXIT("")
}

void SatelliteServer::start()
{
    ENTER("")

    if (p || s) return;
    p = new QValueSpacePublisher ("/server/satellite");
    p->setValue("inview",_inview);
    p->setValue("inuse",_inuse);

    s = QGeoSatelliteInfoSource::createDefaultSource(this);
    if (s) {
        connect(s, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(onInViewChanged(QList<QGeoSatelliteInfo>)));
        connect(s, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(onInUseChanged(QList<QGeoSatelliteInfo>)));
        s->startUpdates();
    }

    emit started();

    EXIT("")
}

void SatelliteServer::stop()
{
    ENTER("")

    if (s) s->stopUpdates();

    delete s;
    delete p;
    s = 0;
    p = 0;
    emit stopped();

    EXIT("")
}
