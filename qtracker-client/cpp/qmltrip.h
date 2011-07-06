#ifndef QMLTRIP_H
#define QMLTRIP_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>

#include "qmlwaypoint.h"
#include "qmlroute.h"
#include "qmltrack.h"

class qmlTrip: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int tripid READ tripid WRITE setTripid NOTIFY tripidChanged)
    void     setTripid(int value)
    { _tripid = value; emit tripidChanged(); }
    int      tripid()
    { return _tripid; }

    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    void     setLimit(int value);
    int      limit()
    { return _limit; }

    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    void     setOffset(int value);
    int      offset()
    { return _offset; }

    Q_PROPERTY(QDateTime triptime READ triptime WRITE setTriptime NOTIFY triptimeChanged)
    void setTriptime(QDateTime value)
    { _triptime = value; emit triptimeChanged(); }
    QDateTime triptime()
    { return _triptime; }

    Q_PROPERTY(double tripdist READ tripdist WRITE setTripdist NOTIFY tripdistChanged)
    void setTripdist(double value)
    { _tripdist = value; emit tripdistChanged(); }
    double tripdist()
    { return _tripdist; }

    Q_PROPERTY(double ascent READ ascent WRITE setAscent NOTIFY ascentChanged)
    void setAscent(double value)
    { _ascent = value; emit ascentChanged(); }
    double ascent()
    { return _ascent; }

    Q_PROPERTY(double descent READ descent WRITE setDescent NOTIFY descentChanged)
    void setDescent(double value)
    { _descent = value; emit descentChanged(); }
    double descent()
    { return _descent; }

    Q_PROPERTY(double altmin READ altmin WRITE setAltmin NOTIFY altminChanged)
    void setAltmin(double value)
    { _altmin = value; emit altminChanged(); }
    double altmin()
    { return _altmin; }

    Q_PROPERTY(double altmax READ altmax WRITE setAltmax NOTIFY altmaxChanged)
    void setAltmax(double value)
    { _altmax = value; emit altmaxChanged(); }
    double altmax()
    { return _altmax; }

    Q_PROPERTY(double altavg READ altavg WRITE setAltavg NOTIFY altavgChanged)
    void setAltavg(double value)
    { _altavg = value; emit altavgChanged(); }
    double altavg()
    { return _altavg; }

    Q_PROPERTY(double speedmin READ speedmin WRITE setSpeedmin NOTIFY speedminChanged)
    void setSpeedmin(double value)
    { _speedmin = value; emit speedminChanged(); }
    double speedmin()
    { return _speedmin; }

    Q_PROPERTY(double speedmax READ speedmax WRITE setSpeedmax NOTIFY speedmaxChanged)
    void setSpeedmax(double value)
    { _speedmax = value; emit speedmaxChanged(); }
    double speedmax()
    { return _speedmax; }

    Q_PROPERTY(double speedavg READ speedavg WRITE setSpeedavg NOTIFY speedavgChanged)
    void setSpeedavg(double value)
    { _speedavg = value; emit speedavgChanged(); }
    double speedavg()
    { return _speedavg; }

    Q_PROPERTY(QDeclarativeListProperty<qmlWaypoint> waypoints READ waypoints CONSTANT)
    QDeclarativeListProperty<qmlWaypoint> waypoints();

    Q_PROPERTY(QDeclarativeListProperty<qmlRoute> routes READ routes CONSTANT)
    QDeclarativeListProperty<qmlRoute> routes();

    Q_PROPERTY(QDeclarativeListProperty<qmlTrack> tracks READ tracks CONSTANT)
    QDeclarativeListProperty<qmlTrack> tracks();

    qmlTrip();
    qmlTrip(int id);
    qmlTrip(const QSqlQuery& q);

    Q_INVOKABLE void selectWaypoints(int offset=0, int limit=10);
    Q_INVOKABLE void selectRoutes(int offset=0, int limit=10);
    Q_INVOKABLE void selectTracks(int offset=0, int limit=10);
    Q_INVOKABLE void addWaypointReference(int wptid);
    Q_INVOKABLE void addRouteReference(int wptid);
    Q_INVOKABLE void addTrackReference(int wptid);

public slots:
    void save();

private:
    void load(const QSqlQuery& q);
    void select();

signals:
    void nameChanged();
    void tripidChanged();
    void limitChanged();
    void offsetChanged();
    void triptimeChanged();
    void tripdistChanged();
    void ascentChanged();
    void descentChanged();
    void altminChanged();
    void altmaxChanged();
    void altavgChanged();
    void speedminChanged();
    void speedmaxChanged();
    void speedavgChanged();

private:
    QString             _name;
    int                 _tripid;
    int                 _limit;
    int                 _offset;
    QDateTime           _triptime;
    double              _tripdist;
    double              _speedavg;
    double              _speedmin;
    double              _speedmax;
    double              _ascent;
    double              _descent;
    double              _altavg;
    double              _altmin;
    double              _altmax;
    QList<qmlWaypoint*> _wpts;
    QList<qmlRoute*>    _rtes;
    QList<qmlTrack*>    _trks;
};

#endif // QMLTRIP_H
