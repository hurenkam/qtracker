#include <iostream>
#include "daemon.h"
#include <QHostAddress>
#include <QTime>

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
        }
    }
}


Daemon::Daemon ( const QString &address, quint16 port, QObject *parent ) :
    QObject( parent )
{
    srv= new XmlRpcServer;
    if ( srv->listen(QHostAddress(address), port) )
    {
        srv->registerSlot( this, SLOT( start( const QVariant &, const QVariant & ) ) );
        srv->registerSlot( this, SLOT( stop() ) );
        srv->registerSlot( this, SLOT( exit() ) );

        srv->registerSlot( this, SLOT( trip() ) );
        srv->registerSlot( this, SLOT( track() ) );
        srv->registerSlot( this, SLOT( altitude() ) );
        srv->registerSlot( this, SLOT( speed() ) );
        srv->registerSlot( this, SLOT( course() ) );
        srv->registerSlot( this, SLOT( location() ) );
        srv->registerSlot( this, SLOT( time() ) );
        srv->registerSlot( this, SLOT( distance() ) );
        std::cout << QTime::currentTime().toString().toStdString()
        << " Start XML-RPC server. " << "Adress:" <<
        QHostAddress( address ).toString().toStdString()
        << " Port:" << port << std::endl;
    }
}

Daemon::~Daemon()
{
    std::cout << "Delete XML-RPC server..." << std::endl;
}

