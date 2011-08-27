#ifndef LOCATIONPROVIDER_H
#define LOCATIONPROVIDER_H

#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

QTM_USE_NAMESPACE

#define LATMASK  0x01
#define LONMASK  0x02
#define ALTMASK  0x04
#define SPDMASK  0x08
#define CRSMASK  0x10
#define DSTMASK  0x20

class LocationProvider: public QObject
{
    Q_OBJECT

public:
    LocationProvider() : QObject(), _validmask(0), _hysteresis(30), _s(0)
    { start(); }

    ~LocationProvider()
    { stop(); }

    int    validmask()  { return _validmask;  }
    double hysteresis() { return _hysteresis; }
    double latitude()   { return _latitude;   }
    double longitude()  { return _longitude;  }
    double altitude()   { return _altitude;   }
    double speed()      { return _speed;      }
    double course()     { return _course;     }
    double distance()   { return _distance;   }

signals:
    void maskChanged(int);
    void positionChanged(double latitude, double longitude, double altitude);
    void latitudeChanged(double latitude);
    void longitudeChanged(double longitude);
    void altitudeChanged(double altitude);
    void speedChanged(double speed);
    void courseChanged(double course);
    void distanceChanged(double distance);

private slots:
    void onPositionChanged(const QGeoPositionInfo& info);

private:
    void start();
    void stop();
    void updateDistance(const QGeoCoordinate& c);

private:    
    int    _validmask;
    double _hysteresis;
    double _latitude;
    double _longitude;
    double _altitude;
    double _speed;
    double _course;
    double _distance;

    QGeoPositionInfoSource* _s;
    QGeoCoordinate _p;
};

#endif // LOCATIONPROVIDER_H
