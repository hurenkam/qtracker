#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QValueSpacePublisher>
#include <QValueSpaceSubscriber>
#include "compassserver.h"
#include "locationserver.h"
#include "altitudeserver.h"
#include "speedserver.h"
#include "timeserver.h"
#include "monitorserver.h"
#include "satelliteserver.h"
#include "dataserver.h"
#include "commandhandler.h"
#include "tripserver.h"

using namespace QtMobility;

class Track;

class Server : public CommandHandler
{
    Q_OBJECT
public:
/*
    Implemented commands:
    000 None
    001 Stop
    002 Reset all tripdata

    Commands to be implemented:
    ??? Reset specific trip data
    ??? Start track recording
    ??? Stop track recording
    ??? Query track data
    ??? Monitor waypoint
    ??? Monitor track
    ??? Monitor route
*/
    enum Command
    {
        cmdNone=0,
        cmdStop,
        cmdReset,
        cmdTrackStart,
        cmdTrackStop,
    };

public:
    explicit Server(QObject *parent = 0);
    ~Server();

signals:
    void started();
    void stopped();

public slots:
    void start();
    void stop();
    void reset();
    void trackStart(QString name, int interval);
    void trackStart(int id, int interval);
    void trackStop();
    void onCommandAvailable();
    //virtual void onCommand(QVariant cmd);
    virtual void onCommand(int method, QVariantList args);

private:
    void onIntegerCommand(int cmd);

private:
    QValueSpacePublisher*   c;
    QValueSpacePublisher*   publisher;
    QValueSpacePublisher*   trackinfo;
    QValueSpaceSubscriber*  subscriber;
    int command;

    CompassServer*   compass;
    LocationServer*  location;
    AltitudeServer*  altitude;
    SpeedServer*     speed;
    TimeServer*      time;
    MonitorServer*   monitor;
    SatelliteServer* satellite;
    DataServer*      data;
    Track*           track;
    TripServer*      trip;

    QMap<QString, CommandHandler*> handler;
};

#endif // SERVER_H
