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
/*
    qmlWaypoint* wpt;
    while (_wpts.length()) { delete _wpts.first(); _wpts.removeFirst(); }
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT wpt FROM catwaypoints WHERE cat='" + QString::number(_catid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { _wpts.append(new qmlWaypoint(q.value(0).toInt())); }
*/
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
