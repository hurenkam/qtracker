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

    // Helper functions
    Q_INVOKABLE QString number(double r, char c, int d)  { return QString::number(r,c,d);         }
    Q_INVOKABLE QString datadir()                        { return QString("c:/workspace/qmltracker/data/"); }
    //Q_INVOKABLE QString datadir()                        { return QString("e:/data/qmltracker/"); }
    Q_INVOKABLE QString mapdir()                         { return datadir() + QString("maps/");   }
    Q_INVOKABLE QString routedir()                       { return datadir() + QString("routes/"); }
    Q_INVOKABLE QString trackdir()                       { return datadir() + QString("tracks/"); }
    Q_INVOKABLE QString importdir()                      { return datadir() + QString("gpx/");    }
    Q_INVOKABLE QString exportdir()                      { return datadir() + QString("gpx/");    }

    Q_INVOKABLE QStringList files(QString& dir, QString& mask);

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
