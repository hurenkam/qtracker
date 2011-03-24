#include <QObject>
#include <QDebug>
#include "altitudemodel.h"

AltitudeModel::AltitudeModel(QObject *parent): QObject(parent)
{
    _subscriber = new QValueSpaceSubscriber("/trackdaemon/altitude",this);
    connect(_subscriber,SIGNAL(contentsChanged()),this,SLOT(onAltitudeChanged()));
    //_timer.start(1000);
    //connect(&_timer,SIGNAL(timeout()),this,SLOT(onAltitudeChanged()));
}

void AltitudeModel::onAltitudeChanged()
{
    emit currentChanged();
    emit averageChanged();
    emit ascentChanged();
    emit descentChanged();
    emit minChanged();
    emit maxChanged();
}

double AltitudeModel::current()
{
    return _subscriber->value("current").toDouble();
}

double AltitudeModel::average()
{
    return _subscriber->value("average").toDouble();
}

double AltitudeModel::ascent()
{
    return _subscriber->value("ascent").toDouble();
}

double AltitudeModel::descent()
{
    return _subscriber->value("descent").toDouble();
}

double AltitudeModel::max()
{
    return _subscriber->value("max").toDouble();
}

double AltitudeModel::min()
{
    return _subscriber->value("min").toDouble();
}

double AltitudeModel::hysteresis()
{
    return _hysteresis;
}

int AltitudeModel::count()
{
    return _count;
}

void AltitudeModel::onReset()
{
}
