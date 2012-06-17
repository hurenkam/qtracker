#ifndef DISTANCEDATA_H
#define DISTANCEDATA_H

#include <QObject>
#include <QVariant>
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
    QVariant data()                           { return toVariant(); }
    QVariant reset()                          { return QVariant(0); }
    QVariant setHysteresis(const QVariant& h) { return QVariant(0); }

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
