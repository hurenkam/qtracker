#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlRefpoint::qmlRefpoint()
    : _name("refpt")
    , _refid(-1)
    , _mapid(-1)
    , _latitude(0)
    , _longitude(0)
    , _x(0)
    , _y(0)
    , _dirty(true)
{
    ENTER("")
    EXIT("")
}

qmlRefpoint::qmlRefpoint(int id)
    : _name("refpt")
    , _mapid(-1)
    , _refid(-1)
    , _latitude(0)
    , _longitude(0)
    , _x(0)
    , _y(0)
    , _dirty(true)
{
    ENTER("")
    setRefid(id);
    EXIT("")
}

qmlRefpoint::qmlRefpoint(const QSqlQuery& q)
    : _name("refpt")
    , _mapid(-1)
    , _refid(-1)
    , _latitude(0)
    , _longitude(0)
    , _x(0)
    , _y(0)
    , _dirty(true)
{
    load(q);
}

void qmlRefpoint::setRefid(int value)
{
    ENTER("")
    if (value > 0)
    {
        QSqlDatabase& db = qmlDatabase::Db();
        QSqlQuery q("SELECT * FROM mappoints WHERE mappt='" + QString::number(value) + "'",db);
        if (q.next())
            load(q);
    }
    else
    {
        _refid = value;
    }
    emit refidChanged();
    EXIT("")
}

void qmlRefpoint::load(const QSqlQuery& q)
{
    ENTER("")
    _refid     = getIntField(q,"mappt");
    _mapid     = getIntField(q,"mapid");
    _name      = getStringField(q,"name");
    _latitude  = getDoubleField(q,"latitude");
    _longitude = getDoubleField(q,"longitude");
    _x         = getDoubleField(q,"x");
    _y         = getDoubleField(q,"y");
    _dirty     = false;
    EXIT("")
}

void qmlRefpoint::save()
{
    ENTER("")
    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q(db);
    if (_refid>0)
        q.exec("REPLACE INTO mappoints (mappt,mapid,name,latitude,longitude,x,y) VALUES (\"" + QString::number(_refid) + "\",\"" + QString::number(_mapid) + "\",\"" + _name  + "\",\"" + QString::number(_latitude)  + "\",\"" + QString::number(_longitude)  + "\",\"" + QString::number(_x) + "\",\"" + QString::number(_y) + "\")");
    else
        q.exec("INSERT  INTO mappoints (mapid,name,latitude,longitude,x,y) VALUES (\"" + QString::number(_mapid) + "\",\"" + _name  + "\",\"" + QString::number(_latitude)  + "\",\"" + QString::number(_longitude)  + "\",\"" + QString::number(_x) + "\",\"" + QString::number(_y) + "\")");
    _dirty = false;
    EXIT("")
}
