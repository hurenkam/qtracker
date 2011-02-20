#include "datamonitor.h"
#include <QDir>
#include <QUrl>
#include <QString>
#include <QStringList>

using namespace QtMobility;


Q_INVOKABLE int
DataMonitor::heading()
{ if (reading) return reading->azimuth(); else return 0; }

Q_INVOKABLE int
DataMonitor::bearing()
{ return 0; }

// Clock
Q_INVOKABLE QDateTime
DataMonitor::time()
{ return QDateTime::currentDateTime(); }

Q_INVOKABLE QDateTime
DataMonitor::triptime()
{ return QDateTime::currentDateTime(); }

Q_INVOKABLE QDateTime
DataMonitor::eta()
{ return QDateTime::currentDateTime(); }

// Altimeter
Q_INVOKABLE double
DataMonitor::altitude()
{
    double alt = posinfo.coordinate().altitude();
    if (alt != alt) // check for NaN
        return 0;
    else
        return alt;
}

Q_INVOKABLE double
DataMonitor::latitude()
{
    double lat = posinfo.coordinate().latitude();
    if (lat != lat) // check for NaN
        return 0;
    else
        return lat;
}

Q_INVOKABLE double
DataMonitor::longitude()
{
    double lon = posinfo.coordinate().longitude();
    if (lon != lon) // check for NaN
        return 0;
    else
        return lon;
}

// StatusBar
Q_INVOKABLE QString
DataMonitor::position()
{
    QString s = QString::number(latitude(),'g',8) + "  " + QString::number(longitude(),'g',8);
    return s;
}

// Speedometer
Q_INVOKABLE double
DataMonitor::speed()
{
    if (posinfo.hasAttribute(QGeoPositionInfo::GroundSpeed))
        return posinfo.attribute(QGeoPositionInfo::GroundSpeed)*3.6;
    else
        return 0;
}

Q_INVOKABLE double
DataMonitor::distance()
{
    return 1523.9;
}

// Levels
Q_INVOKABLE int
DataMonitor::compassLevel()
{
    return (reading->calibrationLevel()*100);
}

Q_INVOKABLE int
DataMonitor::horizontalLevel()
{
    int result = 0;
    if (posinfo.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
    {
        int r = posinfo.attribute(QGeoPositionInfo::HorizontalAccuracy);
        if (r < 300)
            result = 100 - r/3;
    }
    return result;
}

Q_INVOKABLE int
DataMonitor::verticalLevel()
{
    int result = 0;
    if (posinfo.hasAttribute(QGeoPositionInfo::VerticalAccuracy))
    {
        int r = posinfo.attribute(QGeoPositionInfo::VerticalAccuracy);
        if (r < 300)
            result = 100 - r/3;
    }
    return result;
}

Q_INVOKABLE int
DataMonitor::batteryLevel()
{
    QSystemDeviceInfo info;
    return info.batteryLevel();
}

// Satellites
Q_INVOKABLE void
DataMonitor::satellites()
{ }

Q_INVOKABLE QStringList
DataMonitor::files(QString& dir, QString& mask)
{
    QStringList files;
    QDir directory = QDir(dir);
    files = directory.entryList(QStringList(mask), QDir::Files | QDir::NoSymLinks);
    return files;
}


DataMonitor::DataMonitor() : QObject(), compass(0), reading(0)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerFired()));
    timer.start(1000);

    compass = new QCompass();
    if (compass)
    {
        compass->setDataRate(1);
        if (compass->start())
        {
            reading = compass->reading();
            if (reading)
            {
                readingChanged();
                connect(compass, SIGNAL(readingChanged()), this, SLOT(readingChanged()));
            }
        }
    }

    possource = QGeoPositionInfoSource::createDefaultSource(this);
    if (possource) {
        possource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        possource->setUpdateInterval(500);
        connect(possource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(OnPositionUpdate(QGeoPositionInfo)));
        possource->startUpdates();
    }

    satsource = QGeoSatelliteInfoSource::createDefaultSource(this);
    if (satsource) {
        connect(satsource, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(UpdateSatsInView(QList<QGeoSatelliteInfo>)));
        connect(satsource, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(UpdateSatsInUse(QList<QGeoSatelliteInfo>)));
        satsource->startUpdates();
    }
}


void
DataMonitor::timerFired()
{ emit timeChanged(); }


void
DataMonitor::readingChanged()
{ emit headingChanged(); }


void
DataMonitor::OnPositionUpdate(const QGeoPositionInfo& info)
{
    posinfo = info;
    emit positionChanged();
    emit speedChanged();
    emit altitudeChanged();
}


void
DataMonitor::UpdateSatsInUse(const QList<QGeoSatelliteInfo>& inuse)
{ satsinuse = inuse;   emit satellitesChanged(); }


void
DataMonitor::UpdateSatsInView(const QList<QGeoSatelliteInfo>& inview)
{ satsinview = inview; emit satellitesChanged(); }
