#include <QObject>
#include <QDebug>
#include <QTimer>

#include "qmltimemodel.h"
#include "timemodel_tcp.h"
#include "../../tripdataserver/tripservercommands.h"
#include "helpers.h"

qmlTimeModel::qmlTimeModel(QObject *parent): QObject(parent)
{
    p = &PrivateTimeModel::Instance();
    connect(p,SIGNAL(dataChanged),this,SIGNAL(maskChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(currentChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(elapsedChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(monitorChanged));
}

int    qmlTimeModel::mask()       { return p->mask();       }
QTime  qmlTimeModel::current()    { return p->current();    }
QTime  qmlTimeModel::elapsed()    { return p->elapsed();    }
QTime  qmlTimeModel::monitor()    { return p->monitor();    }
void   qmlTimeModel::reset()      {        p->resetData();  }



PrivateTimeModel* PrivateTimeModel::instance = 0;
PrivateTimeModel::PrivateTimeModel(QObject* parent): CommandCaller(11120,parent)
{
    _timer.setSingleShot(true);
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
    LOG( "PrivateSpeedModel::commandExecuted()" )
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

