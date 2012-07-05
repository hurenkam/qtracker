#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlTrack::qmlTrack()
    : _name("trk")
    , _trkid(-1)
    , _interval(0)
    , _top(0)
    , _left(0)
    , _bottom(0)
    , _right(0)
{
    ENTER("")
}

qmlTrack::qmlTrack(int id)
    : _name("trk")
    , _trkid(-1)
    , _interval(0)
    , _top(0)
    , _left(0)
    , _bottom(0)
    , _right(0)
{
    ENTER(id)
    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q("SELECT * FROM tracks WHERE trk='" + QString::number(id) + "'",db);
    if (q.next())
        load(q);

    EXIT("")
}

qmlTrack::qmlTrack(const QSqlQuery& q)
    : _name("trk")
    , _trkid(-1)
{
    load(q);
}

void
qmlTrack::addTrackPoint(double lat, double lon, double alt)
{
    ENTER(lat << lon << alt)
    if (_trkid<1) save();

    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q(db);
    if (!q.exec("INSERT INTO trackpoints (trk,latitude,longitude,altitude,time) VALUES (\""
               + QString::number(_trkid) + "\",\""
               + QString::number(lat)    + "\",\""
               + QString::number(lon)    + "\",\""
               + QString::number(alt)    + "\",\""
               + QDateTime::currentDateTime().toString(Qt::ISODate) + "\")"))
    {
        LOG("qmlTrack::save(): Unable to add track point." << _trkid << lat << lon << alt << QDateTime::currentDateTime().toString(Qt::ISODate) << q.lastError())
    }

    EXIT("")
}

void
qmlTrack::load(const QSqlQuery& q)
{
    ENTER("")
    _trkid     = getIntField(q,"trk");
    _name      = getStringField(q,"name");
    _top       = getDoubleField(q,"north");
    _left      = getDoubleField(q,"west");
    _bottom    = getDoubleField(q,"south");
    _right     = getDoubleField(q,"east");
    _interval  = getIntField(q,"interval");
    EXIT("")
}

void
qmlTrack::save()
{
    ENTER(_trkid)
    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q(db);
    if (_trkid>0)
    {
        if (!q.exec("REPLACE INTO tracks (trk,name,north,west,south,east,interval) VALUES (\""
               + QString::number(_trkid)    + "\",\""
               + _name                      + "\",\""
               + QString::number(_top)      + "\",\""
               + QString::number(_left)     + "\",\""
               + QString::number(_bottom)   + "\",\""
               + QString::number(_right)    + "\",\""
               + QString::number(_interval) + "\")"))
        {
            LOG("qmlTrack::save(): Unable to save track data." << _trkid << _name << _top << _left << _bottom << _right << _interval << q.lastError())
        }
    }
    else
    {
        if (q.exec("INSERT  INTO tracks (name,north,west,south,east,interval) VALUES (\""
               + _name                      + "\",\""
               + QString::number(_top)      + "\",\""
               + QString::number(_left)     + "\",\""
               + QString::number(_bottom)   + "\",\""
               + QString::number(_right)    + "\",\""
               + QString::number(_interval) + "\")"))
        {
            _trkid = q.lastInsertId().toInt();
            emit trkidChanged();
        }
        else
        {
            LOG("qmlTrack::save(): Unable to insert new track data." << _trkid << _name << _top << _left << _bottom << _right << _interval << q.lastError())
        }
    }
    //_dirty = false;
    EXIT(_trkid)
}
