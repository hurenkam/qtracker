#ifndef SATELLITESERVER_H
#define SATELLITESERVER_H

#include <QObject>
#include <QVariantList>
#include <QValueSpacePublisher>
#include <QGeoSatelliteInfo>
#include <QGeoSatelliteInfoSource>

using namespace QtMobility;

class SatelliteServer : public QObject
{
    Q_OBJECT
public:
    explicit SatelliteServer(QObject *parent = 0);
    ~SatelliteServer();

signals:
    void started();
    void stopped();
    //void inUseChanged();
    //void inViewChanged();

public slots:
    void start();
    void stop();
    void reset() {}
    void onInUseChanged(const QList<QGeoSatelliteInfo>& info);
    void onInViewChanged(const QList<QGeoSatelliteInfo>& info);

private:
    QValueSpacePublisher*    p;
    QGeoSatelliteInfoSource* s;
    QVariantList _inuse;
    QVariantList _inview;
};

#endif // SATELLITESERVER_H
