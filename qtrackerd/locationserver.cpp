#include <QVariant>
#include <QDebug>
#include "locationserver.h"

#define ENABLE_DEBUG
#include "helpers.h"
LocationServer::LocationServer(QObject *parent)
    : CommandHandler(parent)
    , p(0)
    , s(0)
    , i(0)
    , _hysteresis(100)
    , _distance(0)
{
    ENTER("")
    methods << "reset" << "sethysteresis";
    EXIT("")
}

void LocationServer::reset()
{
    _distance = 0;
    emit distanceChanged(_distance);
}

void LocationServer::onCommand(int method, QVariantList args)
{
    ENTER("")

    switch (method)
    {
        case 0:
            reset();
            break;
        case 1:
            if (args.count()>0)
                setHysteresis(args[0].toDouble());
            break;
    }

    EXIT("")
}

void LocationServer::setHysteresis(double h)
{
    ENTER("")
    _hysteresis = h>0? h : 0;
    EXIT("")
}

LocationServer::~LocationServer()
{
    ENTER("")
    if (p) stop();
    EXIT("")
}

void LocationServer::UpdatePosition(const QGeoPositionInfo& info)
{
    ENTER("")

    double lat = info.coordinate().latitude();;
    double lon = info.coordinate().longitude();;
    double alt = info.coordinate().altitude();;

    QVariant v;
    v = lat;
    p->setValue("latitude",v);
    emit latitudeChanged(lat);
    v = lon;
    p->setValue("longitude",v);
    emit longitudeChanged(lon);
    v = alt;
    p->setValue("altitude",v);
    emit altitudeChanged(alt);
    emit positionChanged(lat,lon,alt);

    EXIT("")
}

void LocationServer::UpdateDistance(const QGeoPositionInfo& info)
{
    ENTER("")

    if (!i)
    {
        i = new QGeoPositionInfo(info);
        return;
    }

    double delta = i->coordinate().distanceTo(info.coordinate());
    if (delta > _hysteresis)
    {
        *i = info;
        _distance += delta;
        QVariant v = _distance;
        p->setValue("distance",v);
        emit distanceChanged(_distance);
    }

    EXIT("")
}

void LocationServer::UpdateSpeed(const QGeoPositionInfo& info)
{
    ENTER("")

    if (info.hasAttribute(QGeoPositionInfo::GroundSpeed))
    {
        QVariant v = info.attribute(QGeoPositionInfo::GroundSpeed)*3.6;
        p->setValue("speed",v);
        emit speedChanged(v.toDouble());
    }

    EXIT("")
}

void LocationServer::UpdateDirection(const QGeoPositionInfo& info)
{
    ENTER("")

    if (info.hasAttribute(QGeoPositionInfo::Direction))
    {
        QVariant v = info.attribute(QGeoPositionInfo::Direction);
        p->setValue("heading",v);
        emit directionChanged(v.toDouble());
    }

    EXIT("")
}

void LocationServer::UpdateAccuracy(const QGeoPositionInfo& info)
{
    ENTER("")

    QVariant v;
    if (info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
    {
        v = info.attribute(QGeoPositionInfo::HorizontalAccuracy);
        p->setValue("horizontal",v);
        emit horizontalChanged(v.toDouble());
    }
    if (info.hasAttribute(QGeoPositionInfo::VerticalAccuracy))
    {
        v = info.attribute(QGeoPositionInfo::VerticalAccuracy);
        p->setValue("vertical",v);
        emit verticalChanged(v.toDouble());
    }

    EXIT("")
}

void LocationServer::onPositionChanged(const QGeoPositionInfo& info)
{
    ENTER(info)

    UpdatePosition(info);
    UpdateDistance(info);
    UpdateSpeed(info);
    UpdateDirection(info);
    UpdateAccuracy(info);

    EXIT("")
}

void LocationServer::start()
{
    ENTER("")

    if (p || s) return;
    p = new QValueSpacePublisher ("/server/location");

    double d = 0.0;
    p->setValue("latitude",d);
    p->setValue("longitude",d);
    p->setValue("altitude",d);
    p->setValue("speed",d);
    p->setValue("heading",d);
    p->setValue("horizontal",d);
    p->setValue("vertical",d);
    p->setValue("distance",d);

    s = QGeoPositionInfoSource::createDefaultSource(this);
    if (s) {
        s->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        s->setUpdateInterval(1000);
        connect(s, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(onPositionChanged(QGeoPositionInfo)));
        s->startUpdates();
    }

    emit started();

    EXIT("")
}

void LocationServer::stop()
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
