#include <QObject>
#include <QDebug>
#include <QCompass>
#include <QCompassReading>
#include "compassmodel.h"


//============================================================================================


PrivateCompassModel::PrivateCompassModel(QObject *parent)
    : QObject(parent)
    , _current(0)
    , _bearing(0)
    , _average(0)
    , _count(60)
{
    compass = new QCompass();
    if (compass)
    {
        compass->setDataRate(1);
        if (compass->start())
        {
            reading = compass->reading();
            if (reading)
            {
                onReadingChanged();
                connect(compass, SIGNAL(readingChanged()), this, SLOT(onReadingChanged()));
            }
        }
    }

    connect(compass, SIGNAL(readingChanged()), this, SLOT(onReadingChanged()));
}

void PrivateCompassModel::onReset()
{
    _average=0;
    _bearing=0;
    _quality=0;
    _avglist.clear();
}

void PrivateCompassModel::onReadingChanged() {
    if (reading) _current = reading->azimuth();
    emit currentChanged();
    UpdateAverage();
    UpdateBearing();
    UpdateQuality();
}

void PrivateCompassModel::UpdateAverage()
{
    _avglist.append(_current);
    if (_avglist.count() > _count)
    {
        _average -= _avglist.first()/_count;
        _average += _current/_count;
        _avglist.removeFirst();
    }
    else
    {
        int c = _avglist.count();
        _average = _average * (c-1)/c + _current/c;
    }
    emit averageChanged();
}

void PrivateCompassModel::UpdateBearing() {
}

void PrivateCompassModel::UpdateQuality()
{
    if (reading)
    {
        _quality = reading->calibrationLevel()*100;
        emit qualityChanged();
    }
}


//============================================================================================


PrivateCompassModel *CompassModel::p = 0;

CompassModel::CompassModel(QObject *parent): QObject(parent)
{
    if (!p) p = new PrivateCompassModel();

    connect(p, SIGNAL(currentChanged()), this, SLOT(onCurrentChanged()));
    connect(p, SIGNAL(averageChanged()), this, SLOT(onAverageChanged()));
    connect(p, SIGNAL(bearingChanged()), this, SLOT(onBearingChanged()));
    connect(p, SIGNAL(qualityChanged()), this, SLOT(onQualityChanged()));
}

double CompassModel::current()
{
    return p->getCurrent();
}

double CompassModel::average()
{
    return p->getAverage();
}

double CompassModel::bearing()
{
    return p->getBearing();
}

double CompassModel::quality()
{
    return p->getQuality();
}

void CompassModel::onReset()
{
    p->onReset();
}
