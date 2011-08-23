#include <QObject>
#include <QDebug>
#include <QTimer>

#include "qmlaltitudemodel.h"
#include "altitudemodel_tcp.h"

qmlAltitudeModel::qmlAltitudeModel(QObject *parent): QObject(parent) 
{
    p = PrivateAltitudeModel::Instance();
    connect(p,SIGNAL(dataChanged),this,SIGNAL(currentChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(averageChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(minChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(maxChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(ascentChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(descentChanged));
}

double qmlAltitudeModel::current()    { return _p->current();    }
double qmlAltitudeModel::average()    { return _p->average();    }
double qmlAltitudeModel::max()        { return _p->max()         }
double qmlAltitudeModel::min()        { return _p->min();        }
double qmlAltitudeModel::ascent()     { return _p->ascent();     }
double qmlAltitudeModel::descent()    { return _p->descent();    }
int    qmlAltitudeModel::count()      { return _p->count();      }
double qmlAltitudeModel::hysteresis() { return _p->hysteresis(); } 


PrivateAltitudeModel::PrivateAltitudeModel(QObject* parent): QObject(parent)
{
    _timer.setSingleShot(true);
    _timer.setInterval(1000);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(requestData()));
}

PrivateAltitudeModel::~PrivateAltitudeModel(QObject* parent)
{
}

PrivateAltitudeModel& PrivateAltitudeModel::Instance()
{
    if (!instance) instance = new PrivateAltitudeModel();
    return *instance;
}

void PrivateAltitudeModel::onDataRequest()
{
}

void PrivateAltitudeModel::onDataReady()
{
    emit dataChanged();
}

