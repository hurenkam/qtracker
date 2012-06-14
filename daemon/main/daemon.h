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

    QVariant toVariant() {
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

    QVariant toVariant() {
        QMap<QString, QVariant> r;
	r["mask"]    = mask;
	r["current"] = current;
	r["monitor"] = monitor;
	return r;
    }
};

class Daemon :
    QObject
{
    Q_OBJECT

public:
    Daemon( const QString &address, quint16 port, QObject *parent= 0 );
    ~Daemon();

private slots :
    QVariant start( const QVariant &id, const QVariant &interval )
                            { qDebug() << "Daemon::start()";    return 0; }
    QVariant stop()         { qDebug() << "Daemon::stop()";     return 0; }
    QVariant exit()         { qDebug() << "Daemon::exit()";     return 0; }

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
