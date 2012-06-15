#include <QObject>
#include <QDebug>
#include "compassmodel.h"

#define ENABLE_DEBUG
#include "helpers.h"

CompassModel::CompassModel(QObject *parent): QObject(parent)
{
    ENTER("")

    _azimuth = new QValueSpaceSubscriber ("/trackdaemon/compass/azimuth",this);
    connect(_azimuth,SIGNAL(contentsChanged()),this,SLOT(onAzimuthChanged()));

    _calibration = new QValueSpaceSubscriber("/trackdaemon/compass/calibration",this);
    connect(_calibration, SIGNAL(contentsChanged()),this,SLOT(onCalibrationChanged()));

    _monitor     = new QValueSpaceSubscriber("/trackdaemon/compass/monitor",this);
    connect(_monitor,     SIGNAL(contentsChanged()),this,SLOT(onMonitorChanged()));

    EXIT("")
}

void CompassModel::onAzimuthChanged()
{
    ENTER("")

    emit currentChanged();

    EXIT("")
}

void CompassModel::onCalibrationChanged()
{
    ENTER("")

    emit qualityChanged();

    EXIT("")
}

void CompassModel::onMonitorChanged()
{
    ENTER("")

    emit bearingChanged();

    EXIT("")
}

double CompassModel::current()
{
    ENTER("")

    QVariant v = _azimuth->value();
    return v.toDouble();
}

double CompassModel::average()
{
    ENTER("")

    return _azimuth->value().toDouble();
}

double CompassModel::bearing()
{
    ENTER("")

    return _monitor->value().toDouble();
}

double CompassModel::quality()
{
    ENTER("")

    return _calibration->value().toDouble() * 100;
}

void CompassModel::onReset()
{
    ENTER("")
    EXIT("")
}
