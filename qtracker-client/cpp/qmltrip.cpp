#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlTrip::qmlTrip()
    : _name("trip")
    , _tripid(6)
    , _limit(10)
    , _offset(0)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
}

qmlTrip::qmlTrip(int id)
    : _limit(10)
    , _offset(0)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM trips WHERE trip='" + QString::number(id) + "'",db);
    if (q.next())
        load(q);
    EXIT("")
}

qmlTrip::qmlTrip(const QSqlQuery& q)
    : _limit(10)
    , _offset(0)
{
    ENTER("")
    load(q);
    EXIT("")
}

void qmlTrip::load(const QSqlQuery& q)
{
    ENTER("")
    _tripid =   getIntField      (q,"trip");
    _name =     getStringField   (q,"name");
    _triptime = getDateTimeField (q,"triptime");
    _tripdist = getDoubleField   (q,"tripdist");
    _ascent =   getDoubleField   (q,"ascent");
    _descent =  getDoubleField   (q,"descent");
    _altmin =   getDoubleField   (q,"altmin");
    _altmax =   getDoubleField   (q,"altmax");
    _altavg =   getDoubleField   (q,"altavg");
    _speedmin = getDoubleField   (q,"speedmin");
    _speedmax = getDoubleField   (q,"speedmax");
    _speedavg = getDoubleField   (q,"speedavg");
    EXIT("")
}

void qmlTrip::save()
{
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    if (_tripid>0)
    {
        q.exec("REPLACE INTO trips (trip,name,triptime,tripdist,ascent,descent,altmin,altmax,altavg,speedmin,speedmax,speedavg) VALUES (\""
               + QString::number(_tripid)   + "\",\""
               + _name                      + "\",\""
               + _triptime.toString()       + "\",\""
               + QString::number(_tripdist) + "\",\""
               + QString::number(_ascent)   + "\",\""
               + QString::number(_descent)  + "\",\""
               + QString::number(_altmin)   + "\",\""
               + QString::number(_altmax)   + "\",\""
               + QString::number(_altavg)   + "\",\""
               + QString::number(_speedmin) + "\",\""
               + QString::number(_speedmax) + "\",\""
               + QString::number(_speedavg) + "\")");
    }
    else
    {
        q.exec("INSERT  INTO trips (name,triptime,tripdist,ascent,descent,altmin,altmax,altavg,speedmin,speedmax,speedavg) VALUES (\""
               + _name                      + "\",\""
               + _triptime.toString()       + "\",\""
               + QString::number(_tripdist) + "\",\""
               + QString::number(_ascent)   + "\",\""
               + QString::number(_descent)  + "\",\""
               + QString::number(_altmin)   + "\",\""
               + QString::number(_altmax)   + "\",\""
               + QString::number(_altavg)   + "\",\""
               + QString::number(_speedmin) + "\",\""
               + QString::number(_speedmax) + "\",\""
               + QString::number(_speedavg) + "\")");
        _tripid = q.lastInsertId().toInt();
        emit tripidChanged();
    }
    //_dirty = false;
}

QDeclarativeListProperty<qmlWaypoint>
qmlTrip::waypoints()
{
    ENTER("")
    return QDeclarativeListProperty<qmlWaypoint>(this, _wpts);
}

QDeclarativeListProperty<qmlRoute>
qmlTrip::routes()
{
    ENTER("")
    return QDeclarativeListProperty<qmlRoute>(this, _rtes);
}

QDeclarativeListProperty<qmlTrack>
qmlTrip::tracks()
{
    ENTER("")
    return QDeclarativeListProperty<qmlTrack>(this, _trks);
}

void
qmlTrip::setLimit(int value)
{
    ENTER("")
    _limit = value;
    emit limitChanged();
    EXIT("")
}

void
qmlTrip::setOffset(int value)
{
    ENTER("")
    _offset = value;
    emit offsetChanged();
    EXIT("")
}

void
qmlTrip::selectWaypoints(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"waypoints");
    r.clear();
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT wpt FROM tripwaypoints WHERE trip='" + QString::number(_tripid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlWaypoint(q.value(0).toInt())); }
    EXIT("")
}

void
qmlTrip::selectRoutes(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"routes");
    r.clear();
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT rte FROM triproutes WHERE trip='" + QString::number(_tripid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlRoute(q.value(0).toInt())); }
    EXIT("")
}

void
qmlTrip::selectTracks(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"tracks");
    r.clear();
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT trk FROM triptracks WHERE trip='" + QString::number(_tripid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlTrack(q.value(0).toInt())); }
    EXIT("")
}

void
qmlTrip::select()
{
    ENTER("")
    selectWaypoints();
    selectRoutes();
    selectTracks();
    EXIT("")
}

void qmlTrip::addWaypointReference(int wptid)
{
    ENTER(wptid)
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    q.exec("SELECT tripwpt FROM tripwaypoints WHERE trip='" + QString::number(_tripid) + "' AND wpt='" + QString::number(wptid) + "'");
    if (!q.next())
        q.exec("INSERT INTO tripwaypoints (trip,wpt) VALUES (\""  + QString::number(_tripid)  + "\",\"" + QString::number(wptid) + "\")");
    EXIT(q.size())
}

void qmlTrip::addRouteReference(int rteid)
{
    ENTER(rteid)
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    q.exec("SELECT triprte FROM triproutes WHERE trip='" + QString::number(_tripid) + "' AND rte='" + QString::number(rteid) + "'");
    if (!q.next())
        q.exec("INSERT OR REPLACE INTO triproutes (trip,rte) VALUES (\""  + QString::number(_tripid)  + "\",\"" + QString::number(rteid) + "\")");
    EXIT(q.size())
}

void qmlTrip::addTrackReference(int trkid)
{
    ENTER(trkid)
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    q.exec("SELECT triptrk FROM triptracks WHERE trip='" + QString::number(_tripid) + "' AND trk='" + QString::number(trkid) + "'");
    if (!q.next())
        q.exec("INSERT OR REPLACE INTO triptracks (trip,trk) VALUES (\""  + QString::number(_tripid)  + "\",\"" + QString::number(trkid) + "\")");
    EXIT(q.size())
}
