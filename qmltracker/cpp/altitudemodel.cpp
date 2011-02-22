#include <QObject>
#include <QDebug>
#include "altitudemodel.h"


//============================================================================================


PrivateAltitudeModel::PrivateAltitudeModel(QObject *parent)
    : QObject(parent)
    , _current(0)
    , _previous(0)
    , _hysteresis(25)
    , _count(60)
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

void PrivateAltitudeModel::onReset()
{
    _ascent=0;
    _descent=0;
    _average=0;
    _max=-9999;
    _min=99999;
    _avglist.clear();
}

void PrivateAltitudeModel::OnPositionUpdate(const QGeoPositionInfo& info)
{
    posinfo = info;
    double alt = posinfo.coordinate().altitude();
    if (alt == alt) // check for NaN
    {
        _current = alt;
        emit currentChanged();
        UpdateAverage();
        UpdateMinMax();
        UpdateAscent();
        UpdateDescent();
    }
}

void PrivateAltitudeModel::UpdateAverage()
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

void PrivateAltitudeModel::UpdateAscent()
{
    double delta = _current - _previous;
    if (delta>_hysteresis)
    {
        _ascent += delta;
        _previous = _current;
        emit ascentChanged();
    }
}

void PrivateAltitudeModel::UpdateDescent() {
    double delta = _previous - _current;
    if (delta>_hysteresis)
    {
        _descent += delta;
        _previous = _current;
        emit descentChanged();
    }
}

void PrivateAltitudeModel::UpdateMinMax() {
    if (_current > _max) {
        _max = _current;
        emit maxChanged();
    }
    if (_current < _min) {
        _min = _current;
        emit minChanged();
    }
}


//============================================================================================


PrivateAltitudeModel *AltitudeModel::p = 0;

AltitudeModel::AltitudeModel(QObject *parent): QObject(parent)
{
    //qDebug() << "AltitudeModel()";
    if (!p) p = new PrivateAltitudeModel();

    connect(p, SIGNAL(currentChanged()), this, SLOT(onCurrentChanged()));
    connect(p, SIGNAL(averageChanged()), this, SLOT(onAverageChanged()));
    connect(p, SIGNAL(ascentChanged()),  this, SLOT(onAscentChanged()));
    connect(p, SIGNAL(descentChanged()), this, SLOT(onDescentChanged()));
    connect(p, SIGNAL(maxChanged()),     this, SLOT(onMaxChanged()));
    connect(p, SIGNAL(minChanged()),     this, SLOT(onMinChanged()));
}

double AltitudeModel::current()
{
    return p->getCurrent();
}

double AltitudeModel::average()
{
    return p->getAverage();
}

double AltitudeModel::ascent()
{
    return p->getAscent();
}

double AltitudeModel::descent()
{
    return p->getDescent();
}

double AltitudeModel::max()
{
    return p->getMax();
}

double AltitudeModel::min()
{
    return p->getMin();
}

double AltitudeModel::hysteresis()
{
    return p->getHysteresis();
}

int AltitudeModel::count()
{
    return p->getCount();
}

void AltitudeModel::onReset()
{
    p->onReset();
}
