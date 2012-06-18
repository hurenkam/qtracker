#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QVariant>
#include <QString>
#include "xmlrpcserver.h"

#include "timedata.h"
#include "locationdata.h"
#include "altitudedata.h"
#include "speeddata.h"
#include "coursedata.h"
#include "distancedata.h"
#include "tripstatus.h"
#include "trackstatus.h"

class Daemon :
    public QObject
{
    Q_OBJECT

public:
    Daemon( const QString &address, quint16 port, QObject *parent= 0 );
    ~Daemon();

signals:
    void quit();

private slots :
    QVariant exit()         { quit(); return QVariant(0); }

private:
    XmlRpcServer    *srv;
    TripStatus      _trip;
    TrackStatus     _track;
    AltitudeData    _altitude;
    SpeedData       _speed;
    CourseData      _course;
    LocationData    _location;
    TimeData        _time;
    DistanceData    _distance;
};

#endif // DAEMON_H
