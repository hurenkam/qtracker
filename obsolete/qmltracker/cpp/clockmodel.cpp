#include <QObject>
#include <QDebug>
#include "clockmodel.h"


//============================================================================================


PrivateClockModel::PrivateClockModel(QObject *parent) : QObject(parent)
{
    onReset();
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimerExpired()));
    timer.start(1000);
}

void PrivateClockModel::onReset()
{
    _current = QDateTime::currentDateTime();
    _start   = _current;
    _trip    = QDateTime(_current.date(),QTime(0,0,0));
}

void PrivateClockModel::onTimerExpired()
{
    _current = QDateTime::currentDateTime();
    emit currentChanged();
    UpdateTrip();
}

void PrivateClockModel::UpdateTrip()
{
    _trip = _trip.addSecs(1);
    emit tripChanged();
}


//============================================================================================


PrivateClockModel *ClockModel::p = 0;

ClockModel::ClockModel(QObject *parent): QObject(parent)
{
    if (!p) p = new PrivateClockModel();

    connect(p, SIGNAL(currentChanged()), this, SLOT(onCurrentChanged()));
    connect(p, SIGNAL(tripChanged()),    this, SLOT(onTripChanged()));
}

QDateTime ClockModel::current()
{
    return p->getCurrent();
}

QDateTime ClockModel::trip()
{
    return p->getTrip();
}

void ClockModel::onReset()
{
    p->onReset();
}
