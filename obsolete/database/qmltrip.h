#ifndef QMLTRIP_H
#define QMLTRIP_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>

#include "qmlwaypoint.h"
#include "qmlroute.h"
#include "qmltrack.h"

class qmlTrip: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int tripid READ tripid WRITE setTripid NOTIFY tripidChanged)
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(QDateTime triptime READ triptime WRITE setTriptime NOTIFY triptimeChanged)
    Q_PROPERTY(double tripdist READ tripdist WRITE setTripdist NOTIFY tripdistChanged)
    Q_PROPERTY(double ascent READ ascent WRITE setAscent NOTIFY ascentChanged)
    Q_PROPERTY(double descent READ descent WRITE setDescent NOTIFY descentChanged)
    Q_PROPERTY(double altmin READ altmin WRITE setAltmin NOTIFY altminChanged)
    Q_PROPERTY(double altmax READ altmax WRITE setAltmax NOTIFY altmaxChanged)
    Q_PROPERTY(double altavg READ altavg WRITE setAltavg NOTIFY altavgChanged)
    Q_PROPERTY(double speedmin READ speedmin WRITE setSpeedmin NOTIFY speedminChanged)
    Q_PROPERTY(double speedmax READ speedmax WRITE setSpeedmax NOTIFY speedmaxChanged)
    Q_PROPERTY(double speedavg READ speedavg WRITE setSpeedavg NOTIFY speedavgChanged)
    Q_PROPERTY(QDeclarativeListProperty<qmlWaypoint> waypoints READ waypoints CONSTANT)
    Q_PROPERTY(QDeclarativeListProperty<qmlRoute> routes READ routes CONSTANT)
    Q_PROPERTY(QDeclarativeListProperty<qmlTrack> tracks READ tracks CONSTANT)

public slots:
    void save();
    void setLimit(int value);
    void setOffset(int value);
    void setName(QString value)       { _name = value;     emit nameChanged(_name); }
    void setTripid(int value)         { _tripid = value;   emit tripidChanged(_tripid); }
    void setTriptime(QDateTime value) { _triptime = value; emit triptimeChanged(_triptime); }
    void setTripdist(double value)    { _tripdist = value; emit tripdistChanged(_tripdist); }
    void setAscent(double value)      { _ascent = value;   emit ascentChanged(_ascent); }
    void setDescent(double value)     { _descent = value;  emit descentChanged(_descent); }
    void setAltmin(double value)      { _altmin = value;   emit altminChanged(_altmin); }
    void setAltmax(double value)      { _altmax = value;   emit altmaxChanged(_altmax); }
    void setAltavg(double value)      { _altavg = value;   emit altavgChanged(_altavg); }
    void setSpeedmin(double value)    { _speedmin = value; emit speedminChanged(_speedmin); }
    void setSpeedmax(double value)    { _speedmax = value; emit speedmaxChanged(_speedmax); }
    void setSpeedavg(double value)    { _speedavg = value; emit speedavgChanged(_speedavg); }

public:
    QString  name()      { return _name; }
    int      tripid()    { return _tripid; }
    int      limit()     { return _limit; }
    int      offset()    { return _offset; }
    QDateTime triptime() { return _triptime; }
    double tripdist()    { return _tripdist; }
    double ascent()      { return _ascent; }
    double descent()     { return _descent; }
    double altmin()      { return _altmin; }
    double altmax()      { return _altmax; }
    double altavg()      { return _altavg; }
    double speedmin()    { return _speedmin; }
    double speedmax()    { return _speedmax; }
    double speedavg()    { return _speedavg; }

    QDeclarativeListProperty<qmlWaypoint> waypoints();
    QDeclarativeListProperty<qmlRoute> routes();
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

private:
    void load(const QSqlQuery& q);
    void select();

signals:
    void nameChanged(QString);
    void tripidChanged(int);
    void limitChanged(int);
    void offsetChanged(int);
    void triptimeChanged(QDateTime);
    void tripdistChanged(double);
    void ascentChanged(double);
    void descentChanged(double);
    void altminChanged(double);
    void altmaxChanged(double);
    void altavgChanged(double);
    void speedminChanged(double);
    void speedmaxChanged(double);
    void speedavgChanged(double);

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
