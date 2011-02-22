#include <QObject>
#include <QDebug>
#include "speedmodel.h"


//============================================================================================


PrivateSpeedModel::PrivateSpeedModel(QObject *parent)
    : QObject(parent)
    , _current(0)
//    , _previous(0)
    , _count(60)
    , _hysteresis(25)
{
    onReset();
    possource = QGeoPositionInfoSource::createDefaultSource(this);
    if (possource) {
        possource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        possource->setUpdateInterval(500);
        connect(possource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(OnPositionUpdate(QGeoPositionInfo)));
        possource->startUpdates();
    }
}

void PrivateSpeedModel::onReset()
{
    _average=0;
    _distance=0;
    _max=0;
    _min=9999;
    _avglist.clear();
    prevvalid = false;
}

void PrivateSpeedModel::OnPositionUpdate(const QGeoPositionInfo& info)
{
    posinfo = info;
    if (posinfo.hasAttribute(QGeoPositionInfo::GroundSpeed))
    {
        _current = posinfo.attribute(QGeoPositionInfo::GroundSpeed)*3.6;
        emit currentChanged();
        UpdateAverage();
        UpdateMinMax();
    }
    UpdateDistance();
}

void PrivateSpeedModel::UpdateAverage()
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

void PrivateSpeedModel::UpdateMinMax()
{
    if (_current > _max) {
        _max = _current;
        emit maxChanged();
    }
    if (_current < _min) {
        _min = _current;
        emit minChanged();
    }
}

void PrivateSpeedModel::UpdateDistance()
{
    if (prevvalid)
    {
        double delta = posinfo.coordinate().distanceTo(previnfo.coordinate());
        if (delta > _hysteresis)
        {
            _distance += delta;
            previnfo = posinfo;
            emit distanceChanged();
        }
    }
    else
    {
        previnfo = posinfo;
        prevvalid = true;
    }
}

//============================================================================================


PrivateSpeedModel *SpeedModel::p = 0;

SpeedModel::SpeedModel(QObject *parent): QObject(parent)
{
    if (!p) p = new PrivateSpeedModel();

    connect(p, SIGNAL(currentChanged()),  this, SLOT(onCurrentChanged()));
    connect(p, SIGNAL(averageChanged()),  this, SLOT(onAverageChanged()));
    connect(p, SIGNAL(maxChanged()),      this, SLOT(onMaxChanged()));
    connect(p, SIGNAL(minChanged()),      this, SLOT(onMinChanged()));
    connect(p, SIGNAL(distanceChanged()), this, SLOT(onDistanceChanged()));
}

double SpeedModel::current()
{
    return p->getCurrent();
}

double SpeedModel::average()
{
    return p->getAverage();
}

double SpeedModel::max()
{
    return p->getMax();
}

double SpeedModel::min()
{
    return p->getMin();
}

double SpeedModel::hysteresis()
{
    return p->getHysteresis();
}

double SpeedModel::distance()
{
    return p->getDistance();
}

int SpeedModel::count()
{
    return p->getCount();
}

void SpeedModel::onReset()
{
    p->onReset();
}
