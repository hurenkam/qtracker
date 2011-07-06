#ifndef QMLROUTE_H
#define QMLROUTE_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>
#include "qmlwaypoint.h"
/*
class qmlRoutepoint: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; _dirty=true; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int rptid READ rptid WRITE setRptid NOTIFY rptidChanged)
    void setRptid(int value)
    { _rptid = value; _dirty=true; emit rptidChanged(); }
    int      rptid()
    { return _rptid; }

    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    void setLatitude(double value)
    { _latitude = value; _dirty=true; emit latitudeChanged(); }
    double latitude()
    { return _latitude; }

    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    void setLongitude(double value)
    { _longitude = value; _dirty=true; emit longitudeChanged(); }
    double longitude()
    { return _longitude; }

    Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
    void setAltitude(double value)
    { _altitude = value; _dirty=true; emit altitudeChanged(); }
    double altitude()
    { return _altitude; }

    qmlWaypoint();
    qmlWaypoint(int id);
    qmlWaypoint(const QSqlQuery& q);

signals:
    void nameChanged();
    void rptidChanged();
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();

public slots:
    void save();

private:
    QString    _name;
    int        _rptid;
    double     _latitude;
    double     _longitude;
    double     _altitude;
    bool       _dirty;
};
*/

class qmlRoute: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name  READ name  WRITE setName  NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int     rteid READ rteid WRITE setRteid NOTIFY rteidChanged)
    void     setRteid(int value)
    { _rteid = value; emit rteidChanged(); }
    int      rteid()
    { return _rteid; }

    Q_PROPERTY(double top READ top WRITE setTop NOTIFY topChanged)
    void setTop(double value)
    { _top = value; emit topChanged(); }
    double top()
    { return _top; }

    Q_PROPERTY(double left READ left WRITE setLeft NOTIFY leftChanged)
    void setLeft(double value)
    { _left = value; emit leftChanged(); }
    double left()
    { return _left; }

    Q_PROPERTY(double bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    void setBottom(double value)
    { _bottom = value; emit bottomChanged(); }
    double bottom()
    { return _bottom; }

    Q_PROPERTY(double right READ right WRITE setRight NOTIFY rightChanged)
    void setRight(double value)
    { _right = value; emit rightChanged(); }
    double right()
    { return _right; }

    Q_PROPERTY(QDeclarativeListProperty<qmlWaypoint> routepoints READ routepoints CONSTANT)
    QDeclarativeListProperty<qmlWaypoint> routepoints();


    qmlRoute();
    qmlRoute(int id);
    qmlRoute(const QSqlQuery& q);

    Q_INVOKABLE void selectRoutepoints(int offset=0, int limit=100);


public slots:
    void save();

private:
    void load(const QSqlQuery& q);

signals:
    void nameChanged();
    void rteidChanged();
    void topChanged();
    void leftChanged();
    void bottomChanged();
    void rightChanged();

private:
    QString _name;
    int     _rteid;
    double  _top;
    double  _left;
    double  _bottom;
    double  _right;
    QList<qmlWaypoint*> _rtepts;
};

#endif // QMLROUTE_H
