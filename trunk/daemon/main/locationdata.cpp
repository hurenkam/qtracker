#include <QObject>
#include <QVariant>
#include <QMap>
#include <QString>
#include <QDebug>
#include "locationdata.h"

LocationData::LocationData(QObject *parent)
  : QObject(parent)
  , mask(0)
  , latitude(0.0)
  , longitude(0.0)
  , altitude(0.0)
{
    start();
}

void LocationData::start()
{
    source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        source->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        source->setUpdateInterval(1000);
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(onPositionChanged(QGeoPositionInfo)));
        source->startUpdates();
    }
}

void LocationData::stop()
{
    if (source) source->stopUpdates();
    source = 0;
    mask = 0;
}

void LocationData::onPositionChanged(const QGeoPositionInfo& info)
{
    if (info.isValid())
    {
        const QGeoCoordinate& c = info.coordinate();
        if (c.isValid())
        {
            double lat = c.latitude();
            double lon = c.longitude();
            double alt = c.altitude();
            //qDebug() << "LocationData::onPositionChanged()" << lat << lon << alt;
            if ((latitude != lat) || (longitude != lon))
            {
                latitude  = lat;
                mask |= LATMASK;
                longitude = lon;
                mask |= LONMASK;
            }
            if (altitude != alt)
            {
                altitude = alt;
                mask |= ALTMASK;
            }
            emit positionChanged(lat,lon,alt);
        }
        if (info.hasAttribute(QGeoPositionInfo::GroundSpeed))
        {
            double s = info.attribute(QGeoPositionInfo::GroundSpeed);
            emit speedChanged(s);
        }
        if (info.hasAttribute(QGeoPositionInfo::Direction))
        {
            double c = info.attribute(QGeoPositionInfo::Direction);
            emit courseChanged(c);
        }
    }
}

QVariant LocationData::toVariant()
{
    QMap<QString, QVariant> r;
    r["mask"]      = mask;
    r["latitude"]  = latitude;
    r["longitude"] = longitude;
    r["altitude"]  = altitude;
    return r;
}
