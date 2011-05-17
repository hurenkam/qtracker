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
    , trackinfo(0)
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
    handler["trip"]=trip;
    handler["server"]=this;

    methods << "ping" << "ack" << "stop" << "reset" << "trackstart" << "trackstop";
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
        default:
            LOG("Server::onCommandAvailable(): unknown type " << value)
            break;
    }

    EXIT("")
}

void Server::onCommand(int method, QVariantList args)
{
    switch(method)
    {
        case 0:
        {
            LOG("ping")

            QVariantMap m;
            QVariantList l;
            m["class"]="server";
            m["method"]="ack";
            m["args"]=l;
            publisher->setValue("ack",m);

            break;
        }
        case 1:
            LOG("ack")
            break;
        case 2:
            LOG("stop")
            stop();
            break;
        case 3:
            LOG("reset")
            reset();
            break;
        case 4:
        {
            LOG("trackstart")
            //QString name;
            int id;
            int interval;
            if (args.count()>1)
            {
                //name = args[0].toString();
                id = args[0].toInt();
                interval = args[1].toInt();
                trackStart(id,interval);
            }
            else
            {
                LOG(" not enough parameters")
            }
            break;
        }
        case 5:
            LOG("trackstop")
            int id;
            if (args.count()>0)
            {
                //name = args[0].toString();
                id = args[0].toInt();
                trackStop();
            }
            else
            {
                LOG(" not enough parameters")
            }
            break;
    }

    EXIT("")
}

void Server::trackStart(QString name, int interval)
{
    ENTER("(" << name << "," << interval << ")")
    if (!track)
    {
        track = data->createTrack(name,interval);
        connect(
            location,
            SIGNAL(positionChanged(double,double,double)),
            track,
            SLOT(addPoint(double,double,double))
        );

        QVariant trackstatus = "recording";
        QVariant trackid = track->getId();
        QVariant trackname = name;
        QVariant trackinterval = interval;
        trackinfo->setValue("status",trackstatus);
        trackinfo->setValue("id",trackid);
        trackinfo->setValue("name",trackname);
        trackinfo->setValue("interval",trackinterval);
    }
    else
    {
        LOG("Recording already active!")
    }
    EXIT("")
}

void Server::trackStart(int id, int interval)
{
    ENTER("(" << id << "," << interval << ")")
    if (!track)
    {
        //track = data->createTrack(name,interval);
        track = data->openTrack(id);
        connect(
            location,
            SIGNAL(positionChanged(double,double,double)),
            track,
            SLOT(addPoint(double,double,double))
        );

        QVariant trackstatus = "recording";
        QVariant trackid = track->getId();
        QVariant trackname = track->getName();
        QVariant trackinterval = interval;
        trackinfo->setValue("status",trackstatus);
        trackinfo->setValue("id",trackid);
        trackinfo->setValue("name",trackname);
        trackinfo->setValue("interval",trackinterval);
    }
    else
    {
        LOG("Recording already active!")
    }
    EXIT("")
}

void Server::trackStop()
{
    ENTER("")
    delete track;
    track = 0;

    int i = 0;
    QVariant trackstatus = QString("idle");
    QVariant trackid = i;
    QVariant trackname = QString("");
    QVariant trackinterval = i;
    trackinfo->setValue("status",trackstatus);
    trackinfo->setValue("id",trackid);
    trackinfo->setValue("name",trackname);
    trackinfo->setValue("interval",trackinterval);

    EXIT("")
}

void Server::start()
{
    ENTER("")

    if (c || publisher || subscriber) return;
    c          = new QValueSpacePublisher  ("/server/command");
    publisher  = new QValueSpacePublisher  ("/server/response");
    trackinfo  = new QValueSpacePublisher  ("/server/track");
    subscriber = new QValueSpaceSubscriber ("/server/command");

    QVariantMap m;
    QVariantList l;
    m["class"]="server";
    m["method"]="ping";
    m["args"]=l;
    publisher->setValue("ack",m);
    c->setValue("cmd",m);
/*
    QVariant value = subscriber->value("ack");
    if (value.type() == QVariant::Map) {
        QVariantMap map = value.toMap();
        LOG("Found Ack: " << map)
        if ( map.contains("class")
          && map.contains("method")
          && map["class"]=="server"
          && map["method"]=="ack")
        {
            LOG("Exiting, server already running.")
            emit stopped();
            return;
        }
    }
*/
    trackStop();
    connect(subscriber,SIGNAL(contentsChanged()),this,SLOT(onCommandAvailable()));

    compass->start();
    location->start();
    altitude->start();
    speed->start();
    time->start();
    monitor->start();
    satellite->start();
    data->start();
    trip = data->getTripServer();
    trip->start();

    QObject::connect(altitude, SIGNAL(ascentChanged(double)),   trip, SLOT(onAltAscentChanged(double)));
    QObject::connect(altitude, SIGNAL(descentChanged(double)),  trip, SLOT(onAltDescentChanged(double)));
    QObject::connect(altitude, SIGNAL(minChanged(double)),      trip, SLOT(onAltMinChanged(double)));
    QObject::connect(altitude, SIGNAL(maxChanged(double)),      trip, SLOT(onAltMaxChanged(double)));
    QObject::connect(altitude, SIGNAL(averageChanged(double)),  trip, SLOT(onAltAverageChanged(double)));

    QObject::connect(speed,    SIGNAL(minChanged(double)),      trip, SLOT(onSpeedMinChanged(double)));
    QObject::connect(speed,    SIGNAL(maxChanged(double)),      trip, SLOT(onSpeedMaxChanged(double)));
    QObject::connect(speed,    SIGNAL(averageChanged(double)),  trip, SLOT(onSpeedAverageChanged(double)));

    QObject::connect(time,     SIGNAL(tripTimeChanged(QTime)),  trip, SLOT(onTripTimeChanged(QTime)));
    QObject::connect(location, SIGNAL(distanceChanged(double)), trip, SLOT(onTripDistanceChanged(double)));

    emit started();
    EXIT("")
}

void Server::reset()
{
    ENTER("")

    trip->stop();
    satellite->reset();
    monitor->reset();
    time->reset();
    speed->reset();
    altitude->reset();
    location->reset();
    compass->reset();
    trip->start();

    EXIT("")
}

void Server::stop()
{
    ENTER("")

    handler.clear();
    trackStop();

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
    delete trackinfo;
    delete subscriber;
    c = 0;
    publisher = 0;
    trackinfo = 0;
    subscriber = 0;

    LOG("emit stopped")
    emit stopped();

    EXIT("")
}
