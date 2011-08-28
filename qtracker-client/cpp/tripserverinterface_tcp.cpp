#include <QObject>
#include <QDebug>
#include <QTimer>

#include "interface/qmltripserverinterface.h"
#include "tripserverinterface_tcp.h"
#include "interface/tripservercommands.h"
//#define ENABLE_DEBUG
#include "helpers.h"

qmlTripServerInterface::qmlTripServerInterface(QObject *parent): QObject(parent)
{
    p = &PrivateTripServerInterface::Instance();
    connect(p,SIGNAL(dataChanged),this,SIGNAL(tripChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(trackChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(tripnameChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(tracknameChanged));
    connect(p,SIGNAL(dataChanged),this,SIGNAL(trackstateChanged));
}

int     qmlTripServerInterface::trip()       { return p->trip();        }
int     qmlTripServerInterface::track()      { return p->track();       }
QString qmlTripServerInterface::tripname()   { return p->tripname();    }
QString qmlTripServerInterface::trackname()  { return p->trackname();   }
QString qmlTripServerInterface::trackstate() { return p->trackstate();  }
void    qmlTripServerInterface::reset()      {        p->resetServer(); }
void    qmlTripServerInterface::stop()       {        p->stopServer();  }

void    qmlTripServerInterface::startTrack(int track, int interval)
{ p->startTrack(track,interval); }

void    qmlTripServerInterface::stopTrack()
{ p->stopTrack(); }



PrivateTripServerInterface* PrivateTripServerInterface::instance = 0;
PrivateTripServerInterface::PrivateTripServerInterface(QObject* parent): CommandCaller(11120,parent)
{
    _timer.setSingleShot(false);
    _timer.setInterval(1000);
    _timer.start();
    connect(&_timer, SIGNAL(timeout()), this, SLOT(requestData()));
}

PrivateTripServerInterface::~PrivateTripServerInterface()
{
}

PrivateTripServerInterface& PrivateTripServerInterface::Instance()
{
    if (!instance) instance = new PrivateTripServerInterface();
    return *instance;
}

void PrivateTripServerInterface::stopServer()
{
    executeCommand(new StopCommand());
}

void PrivateTripServerInterface::resetServer()
{
    executeCommand(new ResetCommand());
}

void PrivateTripServerInterface::startTrack(int track, int interval)
{
    executeCommand(new StartTrackCommand(track,interval));
}

void PrivateTripServerInterface::stopTrack()
{
    executeCommand(new StopTrackCommand());
}

void PrivateTripServerInterface::requestData()
{
    executeCommand(new RequestTripInterfaceData());
}

void PrivateTripServerInterface::commandFailed(QAbstractSocket::SocketError error)
{
    LOG( "PrivateTripServerInterface::commandFailed()" )
}

void PrivateTripServerInterface::commandExecuted(Command* cmd)
{
    LOG( "PrivateTripServerInterface::commandExecuted()" )
    switch (cmd->cmd())
    {
        case cmdStop:
        {
            break;
        }
        case cmdReset:
        {
            break;
        }
        case cmdStartTrack:
        {
            break;
        }
        case cmdStopTrack:
        {
            break;
        }
        case cmdRequestTripInterfaceData:
        {
            RequestTripInterfaceData* rq = dynamic_cast<RequestTripInterfaceData*>(cmd);
            _trip       = rq->trip();
            _track      = rq->track();
            _tripname   = rq->tripname();
            _trackname  = rq->trackname();
            _trackstate = rq->trackstate();
            LOG("PrivateTripServerInterface::onCommandExecuted() cmdRequestTripInterfaceData: " << _trip << _track << _tripname << _trackname << _trackstate)
            emit dataChanged();
            break;
        }
        default:
        {
            LOG("PrivateTripServerInterface::onCommandExecuted(): unsupported command " << cmd->cmd())
            break;
        }
    }
}

