#include <iostream>
#include "daemon.h"
#include <QHostAddress>
#include <QTime>

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

        srv->registerSlot( &_location, SLOT( data() ),                          "/RPC2/location/" );
        srv->registerSlot( &_location, SLOT( reset() ),                         "/RPC2/location/" );

        srv->registerSlot( &_time,     SLOT( data() ),                          "/RPC2/time/" );
        srv->registerSlot( &_time,     SLOT( reset() ),                         "/RPC2/time/" );

        srv->registerSlot( &_altitude, SLOT( data() ),                          "/RPC2/altitude/" );
        srv->registerSlot( &_altitude, SLOT( reset() ),                         "/RPC2/altitude/" );
        srv->registerSlot( &_altitude, SLOT( setHysteresis(const QVariant& ) ), "/RPC2/altitude/" );
        srv->registerSlot( &_altitude, SLOT( setBufferSize(const QVariant& ) ), "/RPC2/altitude/" );

        srv->registerSlot( &_speed,    SLOT( data() ),                          "/RPC2/speed/" );
        srv->registerSlot( &_speed,    SLOT( reset() ),                         "/RPC2/speed/" );
        srv->registerSlot( &_speed,    SLOT( setBufferSize(const QVariant& ) ), "/RPC2/speed/" );

        srv->registerSlot( &_course,   SLOT( data() ),                          "/RPC2/course/" );
        srv->registerSlot( &_course,   SLOT( reset() ),                         "/RPC2/course/" );
        srv->registerSlot( &_course,   SLOT( setBufferSize(const QVariant& ) ), "/RPC2/course/" );

        srv->registerSlot( &_distance, SLOT( data() ),                          "/RPC2/distance/" );
        srv->registerSlot( &_distance, SLOT( reset() ),                         "/RPC2/distance/" );
        srv->registerSlot( &_distance, SLOT( setHysteresis(const QVariant& ) ), "/RPC2/distance/" );

        std::cout << QTime::currentTime().toString().toStdString()
        << " Start XML-RPC server. " << "Adress:" <<
        QHostAddress( address ).toString().toStdString()
        << " Port:" << port << std::endl;
    }

    connect(&_location,SIGNAL(positionChanged(const QGeoPositionInfo &)),&_distance,SLOT(onPositionChanged(const QGeoPositionInfo &)));
    connect(&_location,SIGNAL(positionChanged(double,double,double)),&_altitude,SLOT(onPositionChanged(double,double,double)));
    connect(&_location,SIGNAL(positionChanged(double,double,double)),&_course,SLOT(onPositionChanged(double,double,double)));
    connect(&_location,SIGNAL(courseChanged(double)),&_course,SLOT(onCourseChanged(double)));
    connect(&_location,SIGNAL(speedChanged(double)),&_speed,SLOT(onSpeedChanged(double)));
}

Daemon::~Daemon()
{
    std::cout << "Delete XML-RPC server..." << std::endl;
}
