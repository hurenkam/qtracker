#ifndef LOCATIONDATA_H
#define LOCATIONDATA_H

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

QTM_USE_NAMESPACE

#define LATMASK  0x01
#define LONMASK  0x02
#define ALTMASK  0x04
#define SPDMASK  0x08
#define CRSMASK  0x10
#define DSTMASK  0x20

class LocationData : public QObject
{
    Q_OBJECT
public:
    explicit LocationData(QObject *parent = 0);
    QVariant toVariant();

signals:
    void positionChanged(const QGeoPositionInfo& info);
    void positionChanged(double latitude, double longitude, double altitude);
    void speedChanged(double speed);
    void courseChanged(double course);

public slots:
    void start();
    void stop();

private slots:
    void onPositionChanged(const QGeoPositionInfo& info);

public:
    int     mask;
    double  latitude;
    double  longitude;
    double  altitude;

private:
    QGeoPositionInfoSource* source;
};

#endif // LOCATIONDATA_H
