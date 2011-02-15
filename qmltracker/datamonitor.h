#ifndef DATAMONITOR_H
#define DATAMONITOR_H

#include <QtCore>
#include <QtDeclarative>
#include <QCompass>
#include <QCompassReading>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfo>
#include <QGeoSatelliteInfoSource>
#include <QSystemDeviceInfo>

using namespace QtMobility;

class Position: public QObject
{
    Q_OBJECT
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY positionChanged)
    Q_PROPERTY(double latitude  READ latitude  WRITE setLatitude  NOTIFY positionChanged)

private:
    double _longitude;
    double _latitude;

signals:
    void positionChanged();

public:
    double longitude() const
    { return _longitude; }

    double latitude()  const
    { return _latitude;  }

    void   setLongitude(double value)
    { _longitude = value; emit positionChanged(); }

    void   setLatitude(double value)
    { _latitude = value;  emit positionChanged(); }

    Position()
    { setLatitude(0); setLongitude(0); }

    Position(const QGeoPositionInfo& posinfo) : QObject()
    { setLatitude(posinfo.coordinate().latitude()); setLongitude(posinfo.coordinate().longitude()); }

    Position(const Position& pos) : QObject()
    { setLatitude(pos.latitude()); setLongitude(pos.longitude()); }

    Position operator=(const Position& pos)
    { return Position(pos); }
};

class DataMonitor: public QObject
{
    Q_OBJECT

private:
    QTimer                   timer;
    QCompass*                compass;
    QCompassReading*         reading;
    QGeoPositionInfoSource*  possource;
    QGeoSatelliteInfoSource* satsource;
    QGeoPositionInfo         posinfo;
    QList<QGeoSatelliteInfo> satsinuse;
    QList<QGeoSatelliteInfo> satsinview;

signals:
    void headingChanged();
    void timeChanged();
    void positionChanged();
    void speedChanged();
    void altitudeChanged();
    void satellitesChanged();

public:
    // Compass
    Q_INVOKABLE int heading();
    Q_INVOKABLE int bearing();

    // Clock
    Q_INVOKABLE QDateTime time();
    Q_INVOKABLE QDateTime triptime();
    Q_INVOKABLE QDateTime eta();

    // Altimeter
    Q_INVOKABLE double altitude();

    // Map
    Q_INVOKABLE double latitude();
    Q_INVOKABLE double longitude();

    // Statusbar
    Q_INVOKABLE QString position();

    // Speedometer
    Q_INVOKABLE double speed();
    Q_INVOKABLE double distance();

    // Level indicators
    Q_INVOKABLE int compassLevel();
    Q_INVOKABLE int horizontalLevel();
    Q_INVOKABLE int verticalLevel();
    Q_INVOKABLE int batteryLevel();

    // Satellite positions
    Q_INVOKABLE void satellites();

    // Constructor
    DataMonitor();

private slots:
    void timerFired();
    void readingChanged();
    void OnPositionUpdate(const QGeoPositionInfo& info);
    void UpdateSatsInUse(const QList<QGeoSatelliteInfo>& inuse);
    void UpdateSatsInView(const QList<QGeoSatelliteInfo>& inview);
};

#endif // DATAMONITOR_H
