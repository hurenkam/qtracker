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

class TripStatus
{
public:
    int     id;
    QString name;
    QString state;

    TripStatus()
        : id(0)
        , name("trip")
        , state("idle")
    {}

    QVariant toVariant()
    {
        QMap<QString, QVariant> r;
	r["id"]    = id;
	r["name"]  = name;
	r["state"] = state;
	return r;
    }
};

class TrackStatus
{
public:
    int     id;
    QString name;
    QString state;

    TrackStatus()
        : id(0)
        , name("track")
        , state("idle")
    {}

    QVariant toVariant() {
        QMap<QString, QVariant> r;
	r["id"]    = id;
	r["name"]  = name;
	r["state"] = state;
	return r;
    }
};

class Daemon :
    public QObject
{
    Q_OBJECT

public:
    Daemon( const QString &address, quint16 port, QObject *parent= 0 );
    ~Daemon();

signals:
    void startTrack( int id, double interval );
    void stopTrack();
    void quit();

private slots :
    QVariant start( const QVariant &id, const QVariant &interval )
                            { startTrack( id.toInt(), interval.toDouble() ); return QVariant(0); }
    QVariant stop()         { stopTrack(); return QVariant(0); }
    QVariant exit()         { quit(); return QVariant(0); }

    QVariant trip()         { return _trip.toVariant(); }
    QVariant track()        { return _track.toVariant(); }
    QVariant altitude()     { return _altitude.toVariant(); }
    QVariant speed()        { return _speed.toVariant(); }
    QVariant course()       { return _course.toVariant(); }
    QVariant location()     { return _location.toVariant(); }
    QVariant time()         { return _time.toVariant(); }
    QVariant distance()     { return _distance.toVariant(); }

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
