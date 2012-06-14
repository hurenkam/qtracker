#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QTime>
#include "xmlrpcserver.h"

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

class AltitudeData
{
public:
    int     mask;
    double  current;
    double  average;
    double  minimum;
    double  maximum;
    double  ascent;
    double  descent;
    double  monitor;

    AltitudeData()
        : mask(0)
        , current(0.0)
        , average(0.0)
        , minimum(0.0)
        , maximum(0.0)
        , ascent (0.0)
        , descent(0.0)
        , monitor(0.0)
    {}

    QVariant toVariant() {
        QMap<QString, QVariant> r;
	r["mask"]    = mask;
	r["current"] = current;
	r["average"] = average;
	r["minimum"] = minimum;
	r["maximum"] = maximum;
	r["ascent"]  = ascent;
	r["descent"] = descent;
	r["monitor"] = monitor;
	return r;
    }
};

class SpeedData
{
public:
    int     mask;
    double  current;
    double  average;
    double  minimum;
    double  maximum;

    SpeedData()
        : mask(0)
        , current(0.0)
        , average(0.0)
        , minimum(0.0)
        , maximum(0.0)
    {}

    QVariant toVariant() {
        QMap<QString, QVariant> r;
	r["mask"]    = mask;
	r["current"] = current;
	r["average"] = average;
	r["minimum"] = minimum;
	r["maximum"] = maximum;
	return r;
    }
};

class CourseData
{
public:
    int     mask;
    double  current;
    double  average;
    double  monitor;

    CourseData()
        : mask(0)
        , current(0.0)
        , average(0.0)
        , monitor(0.0)
    {}

    QVariant toVariant() {
        QMap<QString, QVariant> r;
	r["mask"]    = mask;
	r["current"] = current;
	r["average"] = average;
	r["monitor"] = monitor;
	return r;
    }
};

class LocationData
{
public:
    int     mask;
    double  latitude;
    double  longitude;
    double  altitude;

    LocationData()
        : mask(0)
        , latitude(0.0)
        , longitude(0.0)
        , altitude(0.0)
    {}

    QVariant toVariant() {
        QMap<QString, QVariant> r;
	r["mask"]      = mask;
	r["latitude"]  = latitude;
	r["longitude"] = longitude;
	r["altitude"]  = altitude;
	return r;
    }
};

class TimeData
{
public:
    int        mask;
    QDateTime  current;
    QDateTime  elapsed;
    QDateTime  monitor;

    TimeData()
        : mask(0)
        , current(QDateTime::currentDateTime())
        , elapsed(QDateTime::currentDateTime())
        , monitor(QDateTime::currentDateTime())
    {}

    QVariant toVariant() {
        QMap<QString, QVariant> r;
	r["mask"]    = mask;
        r["current"] = current;
        r["elapsed"] = elapsed;
        r["monitor"] = monitor;
	return r;
    }
};

class DistanceData
{
public:
    int    mask;
    double current;
    double monitor;

    DistanceData()
        : mask(0)
        , current(0.0)
        , monitor(0.0)
    {}

    QVariant toVariant() {
        QMap<QString, QVariant> r;
	r["mask"]    = mask;
	r["current"] = current;
	r["monitor"] = monitor;
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
                            { qDebug() << "Daemon::start()";    startTrack( id.toInt(), interval.toDouble() ); return QVariant(0); }
    QVariant stop()         { qDebug() << "Daemon::stop()";     stopTrack(); return QVariant(0); }
    QVariant exit()         { qDebug() << "Daemon::exit()";     quit(); return QVariant(0); }

    QVariant trip()         { qDebug() << "Daemon::trip()";     return _trip.toVariant(); }
    QVariant track()        { qDebug() << "Daemon::track()";    return _track.toVariant(); }
    QVariant altitude()     { qDebug() << "Daemon::altitude()"; return _altitude.toVariant(); }
    QVariant speed()        { qDebug() << "Daemon::speed()";    return _speed.toVariant(); }
    QVariant course()       { qDebug() << "Daemon::course()";   return _course.toVariant(); }
    QVariant location()     { qDebug() << "Daemon::location()"; return _location.toVariant(); }
    QVariant time()         { qDebug() << "Daemon::time()";     return _time.toVariant(); }
    QVariant distance()     { qDebug() << "Daemon::distance()"; return _distance.toVariant(); }

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
