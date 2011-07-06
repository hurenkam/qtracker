#ifndef QMLCATEGORY_H
#define QMLCATEGORY_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QDeclarativeListProperty>

#include "qmlwaypoint.h"
#include "qmlroute.h"
#include "qmltrack.h"

class qmlCategory: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void     setName(QString value)
    { _name = value; emit nameChanged(); }
    QString  name()
    { return _name; }

    Q_PROPERTY(int catid READ catid WRITE setCatid NOTIFY catidChanged)
    void     setCatid(int value)
    { _catid = value; emit catidChanged(); }
    int      catid()
    { return _catid; }

    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    void     setLimit(int value);
    int      limit()
    { return _limit; }

    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    void     setOffset(int value);
    int      offset()
    { return _offset; }

    Q_PROPERTY(QDeclarativeListProperty<qmlWaypoint> waypoints READ waypoints CONSTANT)
    QDeclarativeListProperty<qmlWaypoint> waypoints();

    Q_PROPERTY(QDeclarativeListProperty<qmlRoute> routes READ routes CONSTANT)
    QDeclarativeListProperty<qmlRoute> routes();

    Q_PROPERTY(QDeclarativeListProperty<qmlTrack> tracks READ tracks CONSTANT)
    QDeclarativeListProperty<qmlTrack> tracks();

    qmlCategory();
    qmlCategory(int id);
    qmlCategory(const QSqlQuery& q);

    Q_INVOKABLE void selectWaypoints(int offset=0, int limit=100);
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
    void catidChanged();
    void limitChanged();
    void offsetChanged();

private:
    QString             _name;
    int                 _catid;
    int                 _limit;
    int                 _offset;
    QList<qmlWaypoint*> _wpts;
    QList<qmlRoute*>    _rtes;
    QList<qmlTrack*>    _trks;
};

#endif // QMLCATEGORY_H
