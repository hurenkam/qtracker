#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlRefpoint::qmlRefpoint()
    : _name("wpt")
    , _refid(-1)
    , _latitude(0)
    , _longitude(0)
{
    ENTER("")
    EXIT("")
}

qmlRefpoint::qmlRefpoint(int id)
    : _name("refpt")
    , _refid(-1)
    , _latitude(0)
    , _longitude(0)
    , _x(0)
    , _y(0)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM mappoints WHERE mappt='" + QString::number(id) + "'",db);
    if (q.next())
    {
        _refid = id;
        _name = getStringField(q,"name");
        _latitude  = getDoubleField(q,"latitude");
        _longitude = getDoubleField(q,"longitude");
        _x = getDoubleField(q,"x");
        _y = getDoubleField(q,"y");
    }
    EXIT("")
}

qmlRefpoint::qmlRefpoint(const QSqlQuery& q)
{
    ENTER("")
    _refid     = getIntField(q,"mappt");
    _name      = getStringField(q,"name");
    _latitude  = getDoubleField(q,"latitude");
    _longitude = getDoubleField(q,"longitude");
    _x = getDoubleField(q,"x");
    _y = getDoubleField(q,"y");
    EXIT("")
}
