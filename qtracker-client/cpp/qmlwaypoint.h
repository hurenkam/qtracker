#ifndef QMLWAYPOINT_H
#define QMLWAYPOINT_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>

class qmlWaypoint: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int wptid READ wptid WRITE setWptid NOTIFY wptidChanged)
    void setWptid(int value)
    { _wptid = value; emit wptidChanged(); }
    int      wptid()
    { return _wptid; }

    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    void setLatitude(double value)
    { _latitude = value; emit latitudeChanged(); }
    double latitude()
    { return _latitude; }

    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    void setLongitude(double value)
    { _longitude = value; emit longitudeChanged(); }
    double longitude()
    { return _longitude; }

    Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
    void setAltitude(double value)
    { _altitude = value; emit altitudeChanged(); }
    double altitude()
    { return _altitude; }

    qmlWaypoint();
    qmlWaypoint(int id);
    qmlWaypoint(const QSqlQuery& q);

signals:
    void nameChanged();
    void wptidChanged();
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();

private:
    QString    _name;
    int        _wptid;
    double     _latitude;
    double     _longitude;
    double     _altitude;
};

#endif
