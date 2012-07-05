//#include <iostream>
#include <QHostAddress>
#include <QTime>
#include "daemon.h"
#include "qmltrip.h"

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

    connect(&_location,SIGNAL(positionChanged(const QGeoPositionInfo &)), &_distance,SLOT(onPositionChanged(const QGeoPositionInfo &)));
    connect(&_location,SIGNAL(positionChanged(const QGeoPositionInfo &)), &_track,   SLOT(onPositionChanged(const QGeoPositionInfo &)));
    connect(&_location,SIGNAL(positionChanged(double,double,double)),     &_altitude,SLOT(onPositionChanged(double,double,double)));
    connect(&_location,SIGNAL(positionChanged(double,double,double)),     &_course,  SLOT(onPositionChanged(double,double,double)));
    connect(&_location,SIGNAL(courseChanged(double)),                     &_course,  SLOT(onCourseChanged  (double)));
    connect(&_location,SIGNAL(speedChanged(double)),                      &_speed,   SLOT(onSpeedChanged   (double)));

    trip = new qmlTrip();
    trip->setName("trip-" + QDateTime::currentDateTime().toString(Qt::ISODate));
    trip->save();
    _trip.start(trip->tripid(),trip->name());

    connect(&_time,     SIGNAL(timeChanged     (QDateTime)), trip, SLOT(setTriptime (QDateTime)));
    connect(&_distance, SIGNAL(distanceChanged (double)),    trip, SLOT(setTripdist (double)));
    connect(&_altitude, SIGNAL(minimumChanged  (double)),    trip, SLOT(setAltmin   (double)));
    connect(&_altitude, SIGNAL(maximumChanged  (double)),    trip, SLOT(setAltmax   (double)));
    connect(&_altitude, SIGNAL(averageChanged  (double)),    trip, SLOT(setAltavg   (double)));
    connect(&_altitude, SIGNAL(ascentChanged   (double)),    trip, SLOT(setAscent   (double)));
    connect(&_altitude, SIGNAL(descentChanged  (double)),    trip, SLOT(setDescent  (double)));
    connect(&_speed,    SIGNAL(minimumChanged  (double)),    trip, SLOT(setSpeedmin (double)));
    connect(&_speed,    SIGNAL(maximumChanged  (double)),    trip, SLOT(setSpeedmax (double)));
    connect(&_speed,    SIGNAL(averageChanged  (double)),    trip, SLOT(setSpeedavg (double)));

    savetimer.setSingleShot(false);
    savetimer.setInterval(15000);
    savetimer.start();

    connect(&savetimer, SIGNAL(timeout()), trip,    SLOT(save()));
    connect(&savetimer, SIGNAL(timeout()), &_track, SLOT(save()));
}

Daemon::~Daemon()
{
    //std::cout << "Delete XML-RPC server..." << std::endl;
}
