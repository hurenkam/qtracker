#ifndef ALTITUDEDATA_H
#define ALTITUDEDATA_H

#include <QObject>
#include <QVariant>
#include "xmlrpcserver.h"

#define CURMASK 0x01
#define AVGMASK 0x02
#define MINMASK 0x04
#define MAXMASK 0x08
#define ASCMASK 0x10
#define DESMASK 0x20

class AltitudeData : public QObject
{
    Q_OBJECT
public:
    explicit AltitudeData(QObject *parent = 0);
    void registerApi(XmlRpcServer* srv);

signals:
    void minimumChanged(double value);
    void maximumChanged(double value);
    void averageChanged(double value);
    void ascentChanged (double value);
    void descentChanged(double value);

public slots:
    void onPositionChanged(double lat, double lon, double alt);
    QVariant data();
    QVariant reset()                          { return QVariant(0); }
    QVariant setHysteresis(const QVariant& h) { return QVariant(0); }
    QVariant setBufferSize(const QVariant& s) { return QVariant(0); }

public:
    int     mask;
    double  current;
    double  average;
    double  minimum;
    double  maximum;
    double  ascent;
    double  descent;
    double  monitor;

private:
    void updateAverage();
    void updateMinMax();
    void updateAscentDescent();

    QList<double> _avglist;
    int           _count;
    double        _hysteresis;
    double        _previous;
};

#endif // ALTITUDEDATA_H
