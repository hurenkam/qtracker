#ifndef TRIPDATASERVER_H
#define TRIPDATASERVER_H

#include <QObject>
#include <QTimer>
#include "commandhandler.h"
#include "interface/qmltrip.h"
#include "interface/qmltrack.h"

class AltitudeProvider;
class SpeedProvider;
class CourseProvider;
class LocationProvider;

class TripDataServer: public CommandHandler
{
    Q_OBJECT

public:
    TripDataServer(QObject* parent=0);
    ~TripDataServer();

signals:
    void timeChanged(QDateTime);
    void quit();

private:
    virtual void handleCommand(quint16 cmd, const QVariantList& args);
    virtual QVariant reportResult() { return result; }
    QDateTime deltaTime();

private:
    AltitudeProvider*  altitude;
    SpeedProvider*     speed;
    CourseProvider*    course;
    LocationProvider*  location;
    QDateTime          starttime;
    QTimer             savetimer;
    bool               isdirty;
    qmlTrip*           trip;
    qmlTrack*          track;
    QVariant           result;
};

#endif // TRIPDATASERVER_H
