#ifndef LOCATIONSERVER_H
#define LOCATIONSERVER_H

#include <QObject>
#include <QValueSpacePublisher>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include "commandhandler.h"

using namespace QtMobility;

class LocationServer : public CommandHandler
{
    Q_OBJECT
public:
    explicit LocationServer(QObject *parent = 0);
    ~LocationServer();

signals:
    void started();
    void stopped();
    void latitudeChanged(double);
    void longitudeChanged(double);
    void altitudeChanged(double);
    void speedChanged(double);
    void directionChanged(double);
    void horizontalChanged(double);
    void verticalChanged(double);
    void positionChanged(double,double,double);
    void distanceChanged(double);

public slots:
    void start();
    void stop();
    void reset();
    void onPositionChanged(const QGeoPositionInfo& info);
    void setHysteresis(double h);
    virtual void onCommand(int method, QVariantList args);

private:
    void UpdatePosition  (const QGeoPositionInfo& info);
    void UpdateSpeed     (const QGeoPositionInfo& info);
    void UpdateDirection (const QGeoPositionInfo& info);
    void UpdateAccuracy  (const QGeoPositionInfo& info);
    void UpdateDistance  (const QGeoPositionInfo& info);

private:
    QValueSpacePublisher*   p;
    QGeoPositionInfoSource* s;
    QGeoPositionInfo*       i;
    double                  _hysteresis;
    double                  _distance;
};

#endif // LOCATIONSERVER_H
