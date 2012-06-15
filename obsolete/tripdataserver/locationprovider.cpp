#include "locationprovider.h"
#define ENABLE_DEBUG
#include "interface/helpers.h"

void LocationProvider::updateDistance(const QGeoCoordinate& c)
{
    if (!_p.isValid())
    {
        _p = c;
        _distance = 0;
        emit distanceChanged(_distance);
    }

    double delta = _p.distanceTo(c);
    if (delta > _hysteresis)
    {
        _distance += (delta/1000);
        _p = c;
        emit distanceChanged(_distance);
        LOG( "LocationProvider::updateDistance(): " << _distance)
    }
}

void LocationProvider::onPositionChanged(const QGeoPositionInfo& info)
{
    LOG( "LocationProvider::onPositionChanged()" )

    if (info.isValid())
    {
        const QGeoCoordinate& c = info.coordinate();
        if (c.isValid())
        {
            double lat = c.latitude();
            double lon = c.longitude();
            double alt = c.altitude();
            if ((_latitude != lat) || (_longitude != lon))
            {
                _latitude  = lat;
                _validmask |= LATMASK;
                _longitude = lon;
                _validmask |= LONMASK;
                LOG( "LocationProvider::onPositionChanged(): Location " << _latitude << _longitude )
                updateDistance(c);
            }
            if (_altitude != alt)
            {
                _altitude = alt;
                _validmask |= ALTMASK;
                LOG( "LocationProvider::onPositionChanged(): Altitude " << _altitude )
            }
            emit positionChanged(_latitude,_longitude,_altitude);
            emit latitudeChanged(_latitude);
            emit longitudeChanged(_longitude);
            emit altitudeChanged(_altitude);
        }

        if (info.hasAttribute(QGeoPositionInfo::GroundSpeed))
        {
            _validmask &= SPDMASK;
            double s = info.attribute(QGeoPositionInfo::GroundSpeed);
            if (_speed != s)
            {
                _speed = s;
                LOG( "LocationProvider::onPositionChanged(): Speed " << _speed )
            }
            emit speedChanged(_speed);
        }

        if (info.hasAttribute(QGeoPositionInfo::Direction))
        {
            _validmask &= CRSMASK;
            double c = info.attribute(QGeoPositionInfo::Direction);
            if (_course != c)
            {
                _course = c;
                LOG( "LocationProvider::onPositionChanged(): Course " << _course )
            }
            emit courseChanged(_course);
        }
    }
}

void LocationProvider::start()
{
    LOG( "LocationProvider::start() >" )

    _s = QGeoPositionInfoSource::createDefaultSource(this);
    if (_s) {
        _s->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        _s->setUpdateInterval(1000);
        connect(_s, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(onPositionChanged(QGeoPositionInfo)));
        _s->startUpdates();
    }

    LOG( "LocationProvider::start() <" )
}

void LocationProvider::stop()
{
    LOG( "LocationProvider::stop()" )

    if (_s) _s->stopUpdates();
    _s = 0;
    _validmask = 0;
}
