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
