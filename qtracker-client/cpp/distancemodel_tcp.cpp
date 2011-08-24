#include <QObject>
#include <QDebug>
#include <QTimer>

#include "qmldistancemodel.h"
#include "distancemodel_tcp.h"
#include "../../tripdataserver/tripservercommands.h"
#include "helpers.h"

qmlDistanceModel::qmlDistanceModel(QObject *parent): QObject(parent)
{
    p = &PrivateDistanceModel::Instance();
    connect(p,SIGNAL(dataChanged),this,SIGNAL(maskChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(currentChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(monitorChanged));
}

int    qmlDistanceModel::mask()       { return p->mask();       }
double qmlDistanceModel::current()    { return p->current();    }
double qmlDistanceModel::monitor()    { return p->monitor();    }
void   qmlDistanceModel::reset()      {        p->resetData();  }



PrivateDistanceModel* PrivateDistanceModel::instance = 0;
PrivateDistanceModel::PrivateDistanceModel(QObject* parent): CommandCaller(11120,parent)
{
    _timer.setSingleShot(true);
    _timer.setInterval(1000);
    _timer.start();
    connect(&_timer, SIGNAL(timeout()), this, SLOT(requestData()));
}

PrivateDistanceModel::~PrivateDistanceModel()
{
}

PrivateDistanceModel& PrivateDistanceModel::Instance()
{
    if (!instance) instance = new PrivateDistanceModel();
    return *instance;
}

void PrivateDistanceModel::resetData()
{
    executeCommand(new ResetDistanceData());
}

void PrivateDistanceModel::requestData()
{
    executeCommand(new RequestDistanceData());
}

void PrivateDistanceModel::commandFailed(QAbstractSocket::SocketError error)
{
    LOG( "PrivateDistanceModel::commandFailed()" )
}

void PrivateDistanceModel::commandExecuted(Command* cmd)
{
    LOG( "PrivateSpeedModel::commandExecuted()" )
    switch (cmd->cmd())
    {
        case cmdResetDistanceData: {
            emit dataChanged();
            break;
        }
        case cmdRequestDistanceData:
        {
            RequestDistanceData* rq = dynamic_cast<RequestDistanceData*>(cmd);
            _mask    = rq->mask();
            _current = rq->current();
            _monitor = rq->monitor();
            emit dataChanged();
            break;
        }
        default:
        {
            LOG("PrivateDistanceModel::onCommandExecuted(): unsupported command " << cmd->cmd())
            break;
        }
    }
}

