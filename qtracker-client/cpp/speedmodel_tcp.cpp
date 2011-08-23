#include <QObject>
#include <QDebug>
#include <QTimer>

#include "qmlspeedmodel.h"
#include "speedmodel_tcp.h"
#include "../../tripdataserver/tripservercommands.h"
#include "helpers.h"

qmlSpeedModel::qmlSpeedModel(QObject *parent): QObject(parent)
{
    p = &PrivateSpeedModel::Instance();
    connect(p,SIGNAL(dataChanged),this,SIGNAL(maskChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(currentChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(averageChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(minimumChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(maximumChanged));
}

int    qmlSpeedModel::mask()       { return p->mask();       }
double qmlSpeedModel::current()    { return p->current();    }
double qmlSpeedModel::average()    { return p->average();    }
double qmlSpeedModel::minimum()    { return p->minimum();    }
double qmlSpeedModel::maximum()    { return p->maximum();    }
int    qmlSpeedModel::count()      { return p->count();      }
void   qmlSpeedModel::reset()      {        p->resetData();  }



PrivateSpeedModel* PrivateSpeedModel::instance = 0;
PrivateSpeedModel::PrivateSpeedModel(QObject* parent): CommandCaller(11120,parent)
{
    _timer.setSingleShot(true);
    _timer.setInterval(1000);
    _timer.start();
    connect(&_timer, SIGNAL(timeout()), this, SLOT(requestData()));
}

PrivateSpeedModel::~PrivateSpeedModel()
{
}

PrivateSpeedModel& PrivateSpeedModel::Instance()
{
    if (!instance) instance = new PrivateSpeedModel();
    return *instance;
}

void PrivateSpeedModel::resetData()
{
    executeCommand(new ResetSpeedData());
}

void PrivateSpeedModel::requestData()
{
    executeCommand(new RequestSpeedData());
}

void PrivateSpeedModel::commandFailed(QAbstractSocket::SocketError error)
{
    LOG( "PrivateSpeedModel::commandFailed()" )
}

void PrivateSpeedModel::commandExecuted(Command* cmd)
{
    LOG( "PrivateSpeedModel::commandExecuted()" )
    switch (cmd->cmd())
    {
        case cmdResetSpeedData: break;
        case cmdRequestSpeedData:
        {
            RequestSpeedData* rq = dynamic_cast<RequestSpeedData*>(cmd);
            _mask    = rq->mask();
            _current = rq->current();
            _average = rq->average();
            _minimum = rq->minimum();
            _maximum = rq->maximum();
            emit dataChanged();
            break;
        }
        default:
        {
            LOG("PrivateSpeedModel::onCommandExecuted(): unsupported command " << cmd->cmd())
            break;
        }
    }
}

