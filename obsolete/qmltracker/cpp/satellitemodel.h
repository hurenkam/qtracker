#ifndef SATELLITEMODEL_H
#define SATELLITEMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QGeoSatelliteInfo>
#include <QGeoSatelliteInfoSource>
#include <QAbstractListModel>
#include <QTimer>

using namespace QtMobility;

//QT_BEGIN_HEADER
//QT_MODULE(Declarative)

//class QDeclarativeContext;
//class QModelIndex;
//class PrivateSatelliteModel;

class SatelliteInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool   inuse     READ inuse     NOTIFY inuseChanged)
    Q_PROPERTY(double azimuth   READ azimuth   NOTIFY azimuthChanged)
    Q_PROPERTY(double elevation READ elevation NOTIFY elevationChanged)
    Q_PROPERTY(double strength  READ strength  NOTIFY strengthChanged)

signals:
    void inuseChanged();
    void azimuthChanged();
    void elevationChanged();
    void strengthChanged();

public:
     SatelliteInfo(QObject *parent = 0)
         : QObject(parent)
         , _inuse(false)
         , _azimuth(0)
         , _elevation(0)
         , _strength(0)
     {}

     SatelliteInfo(bool inuse, double strength, double azimuth, double elevation, QObject *parent = 0)
         : QObject(parent)
         , _inuse(inuse)
         , _azimuth(azimuth)
         , _elevation(elevation)
         , _strength(strength)
     {}

     bool   inuse()     const  { return _inuse;     }
     double azimuth()   const  { return _azimuth;   }
     double elevation() const  { return _elevation; }
     double strength()  const  { return _strength;  }

private:
     bool   _inuse;
     double _azimuth;
     double _elevation;
     double _strength;
};

class SatelliteList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<SatelliteInfo> satellites READ satellites NOTIFY satellitesChanged)
    Q_PROPERTY(int count READ satelliteCount NOTIFY countChanged)

signals:
    void satellitesChanged();
    void countChanged();

public:
    SatelliteList(QObject *parent=0);

    QDeclarativeListProperty<SatelliteInfo> satellites();
    int satelliteCount() const;
    SatelliteInfo *satellite(int) const;

private slots:
     void onTimerExpired();
     void onInUseChanged (const QList<QGeoSatelliteInfo>& inuse);
     void onInViewChanged(const QList<QGeoSatelliteInfo>& inview);

private:
    QTimer                   _timer;
    QList<SatelliteInfo *>   _satellites;

    QGeoSatelliteInfoSource* _satsource;
    QList<QGeoSatelliteInfo> _satsinuse;
    QList<QGeoSatelliteInfo> _satsinview;
};
/*
class PrivateSatelliteModel: public QObject
{
    Q_OBJECT

signals:
    void startUpdate();
    void endUpdate();

public:
    explicit PrivateSatelliteModel(QObject *parent=0);

    int inView();
    int inUse();
    QGeoSatelliteInfo getInView(int i);
    QGeoSatelliteInfo getInUse(int i);

private slots:
    void onInUseChanged (const QList<QGeoSatelliteInfo>& inuse);
    void onInViewChanged(const QList<QGeoSatelliteInfo>& inview);

private:
    QGeoSatelliteInfoSource* satsource;
    QList<QGeoSatelliteInfo> satsinuse;
    QList<QGeoSatelliteInfo> satsinview;
};
*/

//QML_DECLARE_TYPE(SatelliteModel)
//QT_END_HEADER

#endif // SATELLITEMODEL_H
