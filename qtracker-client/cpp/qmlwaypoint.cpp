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
    , _dirty(true)
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
    , _dirty(true)
{
    ENTER("")
    if (id > 0)
    {
        QSqlDatabase& db = Database::Db();
        QSqlQuery q("SELECT * FROM waypoints WHERE wpt='" + QString::number(id) + "'",db);
        if (q.next())
        {
            _wptid     = id;
            _name      = getStringField(q,"name");
            _latitude  = getDoubleField(q,"latitude");
            _longitude = getDoubleField(q,"longitude");
            _altitude  = getDoubleField(q,"altitude");
            _dirty     = false;
        }
    }
    EXIT("")
}

qmlWaypoint::qmlWaypoint(const QSqlQuery& q)
    : _dirty(false)
{
    ENTER("")
    _wptid     = getIntField(q,"wpt");
    _name      = getStringField(q,"name");
    _latitude  = getDoubleField(q,"latitude");
    _longitude = getDoubleField(q,"longitude");
    _altitude  = getDoubleField(q,"altitude");
    EXIT("")
}

void qmlWaypoint::save()
{
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    if (_wptid>0)
    {
        q.exec("REPLACE INTO waypoints (wpt,name,latitude,longitude,altitude) VALUES (\"" + QString::number(_wptid) + "\",\"" + _name  + "\",\"" + QString::number(_latitude)  + "\",\"" + QString::number(_longitude)  + "\",\"" + QString::number(_altitude) + "\")");
    }
    else
    {
        q.exec("INSERT  INTO waypoints (name,latitude,longitude,altitude) VALUES (\""  + _name  + "\",\"" + QString::number(_latitude)  + "\",\"" + QString::number(_longitude)  + "\",\"" + QString::number(_altitude) + "\")");
        _wptid = q.lastInsertId().toInt();
        emit wptidChanged();
    }
    _dirty = false;
}
