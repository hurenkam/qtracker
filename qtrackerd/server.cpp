#include <QDebug>
#include <QMetaType>
#include <QMetaObject>
#include "server.h"
#include "track.h"

#define ENABLE_DEBUG
#include "helpers.h"

Server::Server(QObject *parent)
    //: QObject(parent)
    : CommandHandler(parent)
    , c(0)
    , publisher(0)
    , subscriber(0)
    , command(0)
    , track(0)
{
    ENTER("")

    handler["server"] = this;

    compass   = new CompassServer();
    location  = new LocationServer();
    altitude  = new AltitudeServer();
    speed     = new SpeedServer();
    time      = new TimeServer();
    monitor   = new MonitorServer();
    satellite = new SatelliteServer();
    data      = new DataServer();

    QObject::connect(location, SIGNAL(altitudeChanged(double)), altitude, SLOT(onAltitudeChanged(double)));
    QObject::connect(location, SIGNAL(speedChanged(double)),    speed,    SLOT(onSpeedChanged(double)));
    QObject::connect(location, SIGNAL(positionChanged(double,double,double)),
                     monitor, SLOT(onPositionChanged(double,double,double)));
    QObject::connect(location, SIGNAL(distanceChanged(double)),
                     monitor, SLOT(onDistanceChanged(double)));
    QObject::connect(time, SIGNAL(tripTimeChanged(QTime)),
                     monitor, SLOT(onTimeChanged(QTime)));
    QObject::connect(speed, SIGNAL(averageChanged(double)),
                     monitor, SLOT(onSpeedChanged(double)));
    QObject::connect(compass, SIGNAL(averageChanged(double)),
                     monitor, SLOT(onHeadingChanged(double)));

    handler["altitude"]=altitude;
    handler["location"]=location;
    handler["speed"]=speed;
    handler["time"]=time;
    handler["server"]=this;

    methods << "stop" << "reset" << "trackstart" << "trackstop";
    EXIT("")
}

Server::~Server()
{
    ENTER("")
    if (c || publisher || subscriber) stop();

    delete data;
    delete satellite;
    delete monitor;
    delete time;
    delete speed;
    delete altitude;
    delete location;
    delete compass;

    EXIT("")
}
/*
void Server::onIntegerCommand(int cmd)
{
    ENTER("")

    bool setc = true;
    switch (cmd)
    {
        case cmdNone:
            setc = false;
            break;
        case cmdStop:
            stop();
            setc = false;
            break;
        case cmdReset:
            reset();
            break;
        default:
            LOG("Server::onIntegerCommand(): unknown command " << cmd)
            break;
    }

    if (setc)
        c->setValue("cmd",cmdNone);

    EXIT("")
}
*/
void Server::onCommandAvailable()
{
    ENTER("")

    QVariant value = subscriber->value("cmd");

    switch (value.type())
    {
        case QVariant::Map:
        {
            QVariantMap map = value.toMap();
            if (map.contains("class"))
            {
                QString id=map["class"].toString();
                if (handler.contains(id))
                {
                    handler[id]->onCommand(value);
                }
                else
                {
                    LOG("Server::onCommandAvailable(): no handler for class " << map)
                }
            }
            else
            {
                LOG("Server::onCommandAvailable(): missing class " << map)
            }
            break;
        }
        //case QVariant::Int:
        //    onIntegerCommand(value.toInt());
        //    break;
        default:
            LOG("Server::onCommandAvailable(): unknown type " << value)
            break;
    }

    EXIT("")
}
/*
void Server::onCommand(QVariant cmd)
{
    ENTER("")

    QVariantMap map = cmd.toMap();
    if (!map.contains("method")) return;

    QString method = map["class"].toString() + map["method"].toString();
    QVariantList args;
    if (map.contains("args"))
        args = map["args"].toList();

    QStringList supported;
    supported << "trackstart" << "trackstop";
    if (!supported.contains(method)) return;

    switch (supported.indexOf(method))
*/
void Server::onCommand(int method, QVariantList args)
{
    switch(method)
    {
        case 0:
            LOG("stop")
            stop();
            break;
        case 1:
            LOG("reset")
            reset();
            break;
        case 2:
        {
            if (args.count()>1)
            {
                QString name = args[0].toString();
                int interval = args[1].toInt();
                LOG("trackstart(" << name << "," << interval << ")")
                track = data->createTrack(name,interval);
                connect(
                    location,
                    SIGNAL(positionChanged(double,double,double)),
                    track,
                    SLOT(addPoint(double,double,double))
                );
            }
            break;
        }
        case 3:
            LOG("trackstop")
            delete track;
            track = 0;
            break;
    }

    EXIT("")
}

void Server::start()
{
    ENTER("")

    if (c || publisher || subscriber) return;
    c          = new QValueSpacePublisher  ("/server/command");
    publisher  = new QValueSpacePublisher  ("/server/response");
    subscriber = new QValueSpaceSubscriber ("/server/command");

    //int i = -1;
    //c->setValue         ("cmd",i);
    //publisher->setValue ("ack",i);
    QVariantMap m;
    QVariantList l;
    m["class"]="server";
    m["method"]="start";
    m["args"]=l;
    c->setValue("cmd",m);
    publisher->setValue("ack",m);

    connect(subscriber,SIGNAL(contentsChanged()),this,SLOT(onCommandAvailable()));

    compass->start();
    location->start();
    altitude->start();
    speed->start();
    time->start();
    monitor->start();
    satellite->start();
    data->start();

    emit started();

    EXIT("")
}

void Server::reset()
{
    ENTER("")

    satellite->reset();
    monitor->reset();
    time->reset();
    speed->reset();
    altitude->reset();
    location->reset();
    compass->reset();

    EXIT("")
}

void Server::stop()
{
    ENTER("")

    handler.clear();

    data->stop();
    satellite->stop();
    monitor->stop();
    time->stop();
    speed->stop();
    altitude->stop();
    location->stop();
    compass->stop();

    delete c;
    delete publisher;
    delete subscriber;
    c = 0;
    publisher = 0;
    subscriber = 0;

    LOG("emit stopped")
    emit stopped();

    EXIT("")
}
