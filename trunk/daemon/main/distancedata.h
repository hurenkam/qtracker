#ifndef DISTANCEDATA_H
#define DISTANCEDATA_H

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

QTM_USE_NAMESPACE

class DistanceData : public QObject
{
    Q_OBJECT
public:
    explicit DistanceData(QObject *parent = 0);
    
signals:
    
public slots:
    void onPositionChanged(const QGeoPositionInfo& info);

public:
    QVariant toVariant();

    int    mask;
    double current;
    double monitor;

private:
    QGeoCoordinate _p;
    double _hysteresis;
};

#endif // DISTANCEDATA_H
