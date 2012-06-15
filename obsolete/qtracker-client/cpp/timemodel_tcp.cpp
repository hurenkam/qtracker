#include <QObject>
#include <QDebug>
#include <QTimer>

#include "qmltimemodel.h"
#include "timemodel_tcp.h"
#include "../../tripdataserver/interface/tripservercommands.h"
//#define ENABLE_DEBUG
#include "helpers.h"

qmlTimeModel::qmlTimeModel(QObject *parent): QObject(parent)
{
    p = &PrivateTimeModel::Instance();
    LOG("qmlTimeModel::qmlTimeModel(): p=" << p << this)
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(maskChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(currentChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(elapsedChanged()));
    connect(p,SIGNAL(dataChanged()),this,SIGNAL(monitorChanged()));
}

int       qmlTimeModel::mask()       { return p->mask();       }
QDateTime qmlTimeModel::current()    { return p->current();    }
QDateTime qmlTimeModel::elapsed()    { return p->elapsed();    }
QDateTime qmlTimeModel::monitor()    { return p->monitor();    }
void      qmlTimeModel::reset()      {        p->resetData();  }



PrivateTimeModel* PrivateTimeModel::instance = 0;
PrivateTimeModel::PrivateTimeModel(QObject* parent): CommandCaller(11120,parent)
{
    _timer.setSingleShot(false);
    _timer.setInterval(1000);
    _timer.start();
    connect(&_timer, SIGNAL(timeout()), this, SLOT(requestData()));
}

PrivateTimeModel::~PrivateTimeModel()
{
}

PrivateTimeModel& PrivateTimeModel::Instance()
{
    if (!instance) instance = new PrivateTimeModel();
    return *instance;
}

void PrivateTimeModel::resetData()
{
    executeCommand(new ResetTimeData());
}

void PrivateTimeModel::requestData()
{
    executeCommand(new RequestTimeData());
}

void PrivateTimeModel::commandFailed(QAbstractSocket::SocketError error)
{
    LOG( "PrivateTimeModel::commandFailed()" )
}

void PrivateTimeModel::commandExecuted(Command* cmd)
{
    LOG( "PrivateTimeModel::commandExecuted()" )
    switch (cmd->cmd())
    {
        case cmdResetTimeData:
        {
            emit dataChanged();
            break;
        }
        case cmdRequestTimeData:
        {
            RequestTimeData* rq = dynamic_cast<RequestTimeData*>(cmd);
            _mask    = rq->mask();
            _current = rq->current();
            _elapsed = rq->elapsed();
            _monitor = rq->monitor();
            LOG("PrivateTimeModel::onCommandExecuted(): cmdRequestTimeData " << _current << _elapsed << _monitor)
            emit dataChanged();
            break;
        }
        default:
        {
            LOG("PrivateTimeModel::onCommandExecuted(): unsupported command " << cmd->cmd())
            break;
        }
    }
}

