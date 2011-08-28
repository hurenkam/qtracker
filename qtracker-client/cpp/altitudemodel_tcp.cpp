#include <QObject>
#include <QDebug>
#include <QTimer>

#include "qmlaltitudemodel.h"
#include "altitudemodel_tcp.h"
#include "../../tripdataserver/interface/tripservercommands.h"

//#define ENABLE_DEBUG
#include "helpers.h"

qmlAltitudeModel::qmlAltitudeModel(QObject *parent): QObject(parent)
{
    p = &PrivateAltitudeModel::Instance();
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(maskChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(currentChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(averageChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(minimumChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(maximumChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(ascentChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(descentChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(monitorChanged()));
}

int    qmlAltitudeModel::mask()       { return p->mask();       }
double qmlAltitudeModel::current()    { return p->current();    }
double qmlAltitudeModel::average()    { return p->average();    }
double qmlAltitudeModel::minimum()    { return p->minimum();    }
double qmlAltitudeModel::maximum()    { return p->maximum();    }
double qmlAltitudeModel::ascent()     { return p->ascent();     }
double qmlAltitudeModel::descent()    { return p->descent();    }
double qmlAltitudeModel::monitor()    { return p->monitor();    }
int    qmlAltitudeModel::count()      { return p->count();      }
double qmlAltitudeModel::hysteresis() { return p->hysteresis(); }
void   qmlAltitudeModel::reset()      {        p->resetData();  }



PrivateAltitudeModel* PrivateAltitudeModel::instance = 0;
PrivateAltitudeModel::PrivateAltitudeModel(QObject* parent): CommandCaller(11120,parent)
{
    LOG( "PrivateAltitudeModel::PrivateAltitudeModel()" )
    _timer.setSingleShot(false);
    _timer.setInterval(1000);
    _timer.start();
    connect(&_timer, SIGNAL(timeout()), this, SLOT(requestData()));
}

PrivateAltitudeModel::~PrivateAltitudeModel()
{
}

PrivateAltitudeModel& PrivateAltitudeModel::Instance()
{
    if (!instance) instance = new PrivateAltitudeModel();
    return *instance;
}

void PrivateAltitudeModel::resetData()
{
    LOG( "PrivateAltitudeModel::resetData()" )
    executeCommand(new ResetAltitudeData());
}

void PrivateAltitudeModel::requestData()
{
    LOG( "PrivateAltitudeModel::requestData()" )
    executeCommand(new RequestAltitudeData());
}

void PrivateAltitudeModel::commandFailed(QAbstractSocket::SocketError error)
{
    LOG( "PrivateAltitudeModel::commandFailed()" )
}

void PrivateAltitudeModel::commandExecuted(Command* cmd)
{
    LOG( "PrivateAltitudeModel::commandExecuted()" )
    switch (cmd->cmd())
    {
        case cmdResetAltitudeData:
        {
            emit dataChanged();
            break;
        }
        case cmdRequestAltitudeData:
        {
            RequestAltitudeData* rq = dynamic_cast<RequestAltitudeData*>(cmd);
            _mask    = rq->mask();
            _current = rq->current();
            _average = rq->average();
            _minimum = rq->minimum();
            _maximum = rq->maximum();
            _ascent  = rq->ascent();
            _descent = rq->descent();
            _monitor = rq->monitor();
            emit dataChanged();
            break;
        }
        default:
        {
            LOG("PrivateAltitudeModel::onCommandExecuted(): unsupported command " << cmd->cmd())
            break;
        }
    }
}

