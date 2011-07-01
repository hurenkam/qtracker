#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlWaypoint::qmlWaypoint()
    : _name("wpt")
    , _wptid(-1)
    , _latitude(0)
    , _longitude(0)
    , _altitude(0)
{
    ENTER("")
    EXIT("")
}

qmlWaypoint::qmlWaypoint(int id)
    : _name("wpt")
    , _wptid(-1)
    , _latitude(0)
    , _longitude(0)
    , _altitude(0)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM waypoints WHERE wpt='" + QString::number(id) + "'",db);
    if (q.next())
    {
        _wptid = id;
        _name = getStringField(q,"name");
        _latitude  = getDoubleField(q,"latitude");
        _longitude = getDoubleField(q,"longitude");
        _altitude  = getDoubleField(q,"altitude");
    }
    EXIT("")
}

qmlWaypoint::qmlWaypoint(const QSqlQuery& q)
{
    ENTER("")
    _wptid     = getIntField(q,"wpt");
    _name      = getStringField(q,"name");
    _latitude  = getDoubleField(q,"latitude");
    _longitude = getDoubleField(q,"longitude");
    _altitude  = getDoubleField(q,"altitude");
    EXIT("")
}
