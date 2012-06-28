//#include <iostream>
#include "daemon.h"
#include <QHostAddress>
#include <QTime>

Daemon::Daemon ( const QString &address, quint16 port, QObject *parent ) :
    QObject( parent )
{
    srv= new XmlRpcServer;
    if ( srv->listen(QHostAddress(address), port) )
    {
        srv->registerSlot( this, SLOT( exit() ) );

        _trip.registerApi(srv);
        _track.registerApi(srv);
        _location.registerApi(srv);
        _time.registerApi(srv);
        _altitude.registerApi(srv);
        _speed.registerApi(srv);
        _course.registerApi(srv);
        _distance.registerApi(srv);

        //std::cout << QTime::currentTime().toString().toStdString()
        //<< " Start XML-RPC server. " << "Adress:" <<
        //QHostAddress( address ).toString().toStdString()
        //<< " Port:" << port << std::endl;
    }

    connect(&_location,SIGNAL(positionChanged(const QGeoPositionInfo &)),&_distance,SLOT(onPositionChanged(const QGeoPositionInfo &)));
    connect(&_location,SIGNAL(positionChanged(double,double,double)),&_altitude,SLOT(onPositionChanged(double,double,double)));
    connect(&_location,SIGNAL(positionChanged(double,double,double)),&_course,SLOT(onPositionChanged(double,double,double)));
    connect(&_location,SIGNAL(courseChanged(double)),&_course,SLOT(onCourseChanged(double)));
    connect(&_location,SIGNAL(speedChanged(double)),&_speed,SLOT(onSpeedChanged(double)));
}

Daemon::~Daemon()
{
    //std::cout << "Delete XML-RPC server..." << std::endl;
}
