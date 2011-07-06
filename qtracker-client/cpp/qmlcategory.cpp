#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlCategory::qmlCategory()
    : _name("cat")
    , _catid(-1)
    , _limit(10)
    , _offset(0)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    EXIT("")
}

qmlCategory::qmlCategory(int catid)
    : _limit(10)
    , _offset(0)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM categories WHERE cat='" + QString::number(catid) + "'",db);
    if (q.next())
        load(q);
    EXIT("")
}

qmlCategory::qmlCategory(const QSqlQuery& q)
    : _limit(10)
    , _offset(0)
{
    ENTER("")
    load(q);
    EXIT("")
}

void qmlCategory::load(const QSqlQuery& q)
{
    ENTER("")
    _catid = getIntField(q,"cat");
    _name = getStringField(q,"name");
    EXIT("")
}

void qmlCategory::save()
{
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    if (_catid>0)
    {
        q.exec("REPLACE INTO categories (cat,name) VALUES (\"" + QString::number(_catid) + "\",\"" + _name  + "\")");
    }
    else
    {
        q.exec("INSERT  INTO categories (name) VALUES (\""  + _name + "\")");
        _catid = q.lastInsertId().toInt();
        emit catidChanged();
    }
    //_dirty = false;
}

QDeclarativeListProperty<qmlWaypoint>
qmlCategory::waypoints()
{
    ENTER("")
    return QDeclarativeListProperty<qmlWaypoint>(this, _wpts);
}

QDeclarativeListProperty<qmlRoute>
qmlCategory::routes()
{
    ENTER("")
    return QDeclarativeListProperty<qmlRoute>(this, _rtes);
}

QDeclarativeListProperty<qmlTrack>
qmlCategory::tracks()
{
    ENTER("")
    return QDeclarativeListProperty<qmlTrack>(this, _trks);
}

void
qmlCategory::setLimit(int value)
{
    ENTER("")
    _limit = value;
    emit limitChanged();
    EXIT("")
}

void
qmlCategory::setOffset(int value)
{
    ENTER("")
    _offset = value;
    emit offsetChanged();
    EXIT("")
}

void
qmlCategory::selectWaypoints(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"waypoints");
    r.clear();
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT wpt FROM catwaypoints WHERE cat='" + QString::number(_catid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlWaypoint(q.value(0).toInt())); }
    EXIT("")
}

void
qmlCategory::selectRoutes(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"tracks");
    r.clear();
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT trk FROM cattracks WHERE cat='" + QString::number(_catid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlRoute(q.value(0).toInt())); }
    EXIT("")
}

void
qmlCategory::selectTracks(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"tracks");
    r.clear();
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT trk FROM cattracks WHERE cat='" + QString::number(_catid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlTrack(q.value(0).toInt())); }
    EXIT("")
}

void
qmlCategory::select()
{
    ENTER("")
    selectWaypoints();
    selectRoutes();
    selectTracks();
    EXIT("")
}

void qmlCategory::addWaypointReference(int wptid)
{
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    q.exec("INSERT OR REPLACE INTO catwaypoints (cat,wpt) VALUES (\""  + QString::number(_catid)  + "\",\"" + QString::number(wptid) + "\")");
}

void qmlCategory::addRouteReference(int rteid)
{
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    q.exec("INSERT OR REPLACE INTO catroutes (cat,rte) VALUES (\""  + QString::number(_catid)  + "\",\"" + QString::number(rteid) + "\")");
}

void qmlCategory::addTrackReference(int trkid)
{
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    q.exec("INSERT OR REPLACE INTO cattracks (cat,trk) VALUES (\""  + QString::number(_catid)  + "\",\"" + QString::number(trkid) + "\")");
}
