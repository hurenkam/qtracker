#include <QObject>
#include <QCoreApplication>
#include <QtNetwork>
#include <QLocalServer>
#include <QLocalSocket>
#include <stdlib.h>

#include "tripdataserver.h"
#include "interface/command.h"
#include "interface/tripservercommands.h"

#include "altitudeprovider.h"
#include "speedprovider.h"
#include "courseprovider.h"
#include "locationprovider.h"

QTM_USE_NAMESPACE

TripDataServer::TripDataServer(QObject* parent)
    : CommandHandler(11120,parent)
    , altitude(0)
    , speed(0)
    , course(0)
    , location(0)
    , trip(0)
    , track(0)
{
    qDebug() << "TripDataServer::TripDataServer()";

    savetimer.setSingleShot(false);
    savetimer.setInterval(5000);
    savetimer.start();

    trip =      new qmlTrip();
    trip->setName("trip-" + QDateTime::currentDateTime().toString(Qt::ISODate));
    trip->save();

    altitude =  new AltitudeProvider();
    speed =     new SpeedProvider();
    course =    new CourseProvider();
    location =  new LocationProvider();
    starttime = QDateTime::currentDateTime();

    connect(location,SIGNAL(altitudeChanged(double)),altitude,SLOT(onAltitudeChanged(double)));
    connect(location,SIGNAL(speedChanged(double)),speed,SLOT(onSpeedChanged(double)));
    connect(location,SIGNAL(courseChanged(double)),course,SLOT(onCourseChanged(double)));

    connect(this,       SIGNAL(timeChanged(QDateTime)),  trip, SLOT(setTriptime(QDateTime)));
    connect(location,   SIGNAL(distanceChanged(double)), trip, SLOT(setTripdist(double)));
    connect(altitude,   SIGNAL(minimumChanged(double)),  trip, SLOT(setAltmin(double)));
    connect(altitude,   SIGNAL(maximumChanged(double)),  trip, SLOT(setAltmax(double)));
    connect(altitude,   SIGNAL(averageChanged(double)),  trip, SLOT(setAltavg(double)));
    connect(altitude,   SIGNAL(ascentChanged(double)),   trip, SLOT(setAscent(double)));
    connect(altitude,   SIGNAL(descentChanged(double)),  trip, SLOT(setDescent(double)));
    connect(speed,      SIGNAL(minimumChanged(double)),  trip, SLOT(setSpeedmin(double)));
    connect(speed,      SIGNAL(maximumChanged(double)),  trip, SLOT(setSpeedmax(double)));
    connect(speed,      SIGNAL(averageChanged(double)),  trip, SLOT(setSpeedavg(double)));
    connect(&savetimer, SIGNAL(timeout()),               trip, SLOT(save()));
}

TripDataServer::~TripDataServer()
{
    qDebug() << "TripDataServer::~TripDataServer()";
    delete altitude;
    delete speed;
    delete course;
    delete location;
}

QDateTime TripDataServer::deltaTime()
{
    QDateTime current = QDateTime::currentDateTime();
    uint secs = starttime.secsTo(current) % (24*60*60);
    QTime deltatime = QTime(0,0,0).addSecs(secs);
    QDateTime delta = QDateTime(current.date(),deltatime);
    return delta;
}

void TripDataServer::handleCommand(quint16 cmd, const QVariantList& args)
{
    qDebug() << "TripDataServer::handleCommand(" << cmd << args << ")";

    result = QVariant();
    switch (cmd)
    {
    case cmdStop:
        qDebug() << "TripDataServer::handleCommand(): quit";
        emit quit();
        break;
    case cmdReset:
        qDebug() << "TripDataServer::handleCommand(): reset";
        break;
    case cmdStopTrack:
    {
        if (track)
        {
            qDebug() << "TripDataServer::handleCommand(): stopping track recording";
            track->save();
            delete track;
        }
        else
        {
            qDebug() << "TripDataServer::handleCommand(): track stop request ignored because track is not recording.";
        }
        track = 0;
        break;
    }
    case cmdStartTrack:
    {
        if (!track)
        {
            if (args.length()>=2)
            {
                int id = args[0].toInt();
                //int interval = args[1].toInt();
                track = new qmlTrack(id);
                if (track)
                {
                    qDebug() << "TripDataServer::handleCommand(): starting track recording";
                    connect(location,   SIGNAL(positionChanged(double,double,double)), track, SLOT(addTrackPoint(double,double,double)));
                    connect(&savetimer, SIGNAL(timeout()),                             track, SLOT(save()));
                }
                else
                {
                    qDebug() << "TripDataServer::handleCommand(): unable to create track " << id;
                }
            }
            else
            {
                qDebug() << "TripDataServer::handleCommand(): track start request failed because track id and/or interval are missing.";
            }
        }
        else
        {
            qDebug() << "TripDataServer::handleCommand(): track start request ignored because track is already recording.";
        }
        
        break;
    }
    case cmdRequestTripData:
    {
        qDebug() << "TripDataServer::handleCommand(): request trip data";
        RequestTripData rq;
        //rq.set(1,2,3,4,5,6,7,8,9,10,11,12,13,14,QTime(0,0,0),"a","idle","b");
        rq.set(
            1,
            altitude->validmask(),
            altitude->current(),
            altitude->average(),
            altitude->minimum(),
            altitude->maximum(),
            altitude->ascent(),
            altitude->descent(),
            speed->validmask(),
            speed->current(),
            speed->average(),
            speed->minimum(),
            speed->maximum(),
            location->distance(),
            QTime::currentTime(),
            "a","idle","b"
        );
        result = rq.res();
        break;
    }
    case cmdRequestTripInterfaceData:
    {
        qDebug() << "TripDataServer::handleCommand(): trip interface data request";
        RequestTripInterfaceData rq;
        rq.set(
            trip->tripid(),
            track? track->trkid() : -1,
            trip->name(),
            track? track->name() : "<no name>",
            track? QString("recording") : QString("idle")
        );
        result = rq.res();
        break;
    }
    case cmdRequestAltitudeData:
    {
        qDebug() << "TripDataServer::handleCommand(): request altitude data";
        RequestAltitudeData rq;
        rq.set(
            altitude->validmask(),
            altitude->current(),
            altitude->average(),
            altitude->minimum(),
            altitude->maximum(),
            altitude->ascent(),
            altitude->descent(),
            0 // monitor->altitude()
        );
        result = rq.res();
        break;
    }
    case cmdRequestSpeedData:
    {
        qDebug() << "TripDataServer::handleCommand(): request speed data";
        RequestSpeedData rq;
        rq.set(
            speed->validmask(),
            speed->current(),
            speed->average(),
            speed->minimum(),
            speed->maximum()
            // monitor->speed()
        );
        result = rq.res();
        break;
    }
    case cmdRequestCourseData:
    {
        qDebug() << "TripDataServer::handleCommand(): request course data";
        RequestCourseData rq;
        rq.set(1,location->course(),3,4);
        result = rq.res();
        break;
    }
    case cmdRequestLocationData:
    {
        qDebug() << "TripDataServer::handleCommand(): request location data";
        RequestLocationData rq;
        rq.set(
            location->validmask(),
            location->latitude(),
            location->longitude(),
            location->altitude()
        );
        result = rq.res();
        break;
    }
    case cmdRequestTimeData:
    {
        qDebug() << "TripDataServer::handleCommand(): request time data";
        RequestTimeData rq;
        QDateTime current =QDateTime::currentDateTime();
        QDateTime delta = deltaTime();
        emit timeChanged(delta);
        rq.set(1,current,delta,QDateTime());
        result = rq.res();
        break;
    }
    case cmdRequestDistanceData:
    {
        qDebug() << "TripDataServer::handleCommand(): request distance data";
        RequestDistanceData rq;
        rq.set(1,location->distance(),0);
        result = rq.res();
        break;
    }
    default:
        qDebug() << "TripDataServer::handleCommand(): unknown command " << cmd;
        break;
    }
}
