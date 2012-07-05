#ifndef TRACKSTATUS_H
#define TRACKSTATUS_H

#include <QObject>
#include <QVariant>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QDateTime>
#include "xmlrpcserver.h"

QTM_USE_NAMESPACE

class qmlTrack;
class TrackStatus : public QObject
{
    Q_OBJECT
public:
    explicit TrackStatus(QObject *parent = 0);
    void registerApi(XmlRpcServer* srv);

signals:
    void trackPoint(double lat, double lon, double alt);

public slots:
    void onPositionChanged(const QGeoPositionInfo& info);
    QVariant start(const QVariant& id, const QVariant& name);
    QVariant stop();
    QVariant data();
    void save();

public:
    int     id;
    QString name;
    QString state;

private:
    qmlTrack *_track;
    QGeoCoordinate _pc;
    QDateTime _pt;
};

#endif // TRACKSTATUS_H
