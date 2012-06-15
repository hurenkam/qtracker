#include <QObject>
#include <QDebug>
#include "monitormodel.h"


//============================================================================================


PrivateMonitorModel::PrivateMonitorModel(QObject *parent) : QObject(parent)
{
    onReset();
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeChanged()));
    //connect(&timer, SIGNAL(positionChanged()), this, SLOT(onPositionChanged()));
    timer.start(1000);
}

void PrivateMonitorModel::onReset()
{
    _distance = 0;
    _bearing = 0;
    _time = QDateTime::currentDateTime();
}

void PrivateMonitorModel::onTimeChanged()
{
    _time = QDateTime::currentDateTime();
    emit timeChanged();
}

void PrivateMonitorModel::onPositionChanged()
{
}

//============================================================================================


PrivateMonitorModel *MonitorModel::p = 0;

MonitorModel::MonitorModel(QObject *parent): QObject(parent)
{
    if (!p) p = new PrivateMonitorModel();

    connect(p, SIGNAL(distanceChanged()), this, SLOT(onDistanceChanged()));
    connect(p, SIGNAL(bearingChanged()),  this, SLOT(onBearingChanged()));
    connect(p, SIGNAL(timeChanged()),     this, SLOT(onTimeChanged()));
}

double MonitorModel::distance()
{
    return p->getDistance();
}

double MonitorModel::bearing()
{
    return p->getBearing();
}

QDateTime MonitorModel::time()
{
    return p->getTime();
}

void MonitorModel::onReset()
{
    p->onReset();
}
