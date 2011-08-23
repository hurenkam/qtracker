#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlRoute::qmlRoute()
    : _name("rte")
    , _rteid(-1)
{
    ENTER("")
}

qmlRoute::qmlRoute(int id)
    : _name("rte")
    , _rteid(-1)
{
    ENTER("")
    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q("SELECT * FROM routes WHERE rte='" + QString::number(id) + "'",db);
    if (q.next())
        load(q);

    EXIT("")
}

qmlRoute::qmlRoute(const QSqlQuery& q)
    : _name("rte")
    , _rteid(-1)
{
    load(q);
}

void
qmlRoute::load(const QSqlQuery& q)
{
    ENTER("")
    _rteid     = getIntField(q,"rte");
    _name      = getStringField(q,"name");
    _top       = getDoubleField(q,"top");
    _left      = getDoubleField(q,"left");
    _bottom    = getDoubleField(q,"bottom");
    _right     = getDoubleField(q,"right");
    EXIT("")
}

void qmlRoute::save()
{
    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q(db);
    if (_rteid>0)
    {
        q.exec("REPLACE INTO routes (rte,name,north,west,south,east) VALUES (\"" + QString::number(_rteid) + "\",\"" + _name  + "\",\"" + QString::number(_top)  + "\",\"" + QString::number(_left)  + "\",\"" + QString::number(_bottom)  + "\",\"" + QString::number(_right) + "\")");
    }
    else
    {
        q.exec("INSERT  INTO categories (name) VALUES (\"" + _name  + "\",\"" + QString::number(_top)  + "\",\"" + QString::number(_left)  + "\",\"" + QString::number(_bottom)  + "\",\"" + QString::number(_right) + "\")");
        _rteid = q.lastInsertId().toInt();
        emit rteidChanged();
    }
    //_dirty = false;
}

QDeclarativeListProperty<qmlWaypoint>
qmlRoute::routepoints()
{
    ENTER("")
    return QDeclarativeListProperty<qmlWaypoint>(this, _rtepts);
}

void
qmlRoute::selectRoutepoints(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"routepoints");
    r.clear();
    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q("SELECT wpt FROM routepoints WHERE rte='" + QString::number(_rteid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlWaypoint(q.value(0).toInt())); }
    EXIT("")
}
