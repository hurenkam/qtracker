#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

const QStringList dbsetup = QStringList()
    << "CREATE TABLE IF NOT EXISTS maps          (mapid   INTEGER PRIMARY KEY, name  TEXT,    time TEXT, filename TEXT,    north     NUMBER, east      NUMBER, south    NUMBER, west NUMBER);"
    << "CREATE TABLE IF NOT EXISTS mappoints     (mappt   INTEGER PRIMARY KEY, mapid INTEGER, name TEXT, latitude NUMBER,  longitude NUMBER, x         NUMBER, y        NUMBER);"
    << "CREATE TABLE IF NOT EXISTS waypoints     (wpt     INTEGER PRIMARY KEY, name  TEXT,    time TEXT, latitude NUMBER,  longitude NUMBER, altitude  NUMBER, notes    TEXT);"
    << "CREATE TABLE IF NOT EXISTS routes        (rte     INTEGER PRIMARY KEY, name  TEXT,    time TEXT, north    NUMBER,  east      NUMBER, south     NUMBER, west     NUMBER);"
    << "CREATE TABLE IF NOT EXISTS routepoints   (rtept   INTEGER PRIMARY KEY, rte   INTEGER, name TEXT, time     TEXT,    latitude  NUMBER, longitude NUMBER, altitude NUMBER);"
    << "CREATE TABLE IF NOT EXISTS tracks        (trk     INTEGER PRIMARY KEY, name  TEXT,    time TEXT, interval INTEGER, north     NUMBER, east      NUMBER, south    NUMBER, west NUMBER);"
    << "CREATE TABLE IF NOT EXISTS trackpoints   (trkpt   INTEGER PRIMARY KEY, trk   INTEGER, time TEXT, latitude NUMBER,  longitude NUMBER, altitude  NUMBER, time     TEXT);"
    << "CREATE TABLE IF NOT EXISTS categories    (cat     INTEGER PRIMARY KEY, name  TEXT,    time TEXT, notes    TEXT);"
    << "CREATE TABLE IF NOT EXISTS catwaypoints  (catwpt  INTEGER PRIMARY KEY, cat   INTEGER, wpt  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS catroutes     (catrte  INTEGER PRIMARY KEY, cat   INTEGER, rte  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS cattracks     (cattrk  INTEGER PRIMARY KEY, cat   INTEGER, trk  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS trips         (trip    INTEGER PRIMARY KEY, name  TEXT,    time TEXT, ascent   NUMBER,  descent   NUMBER, altmin    NUMBER, altmax   NUMBER, altavg NUMBER speedmin NUMBER, speedmax NUMBER, speedavg NUMBER, triptime DATETIME, tripdist NUMBER);"
    << "CREATE TABLE IF NOT EXISTS tripwaypoints (tripwpt INTEGER PRIMARY KEY, trip  INTEGER, wpt  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS triproutes    (triprte INTEGER PRIMARY KEY, trip  INTEGER, rte  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS triptracks    (triptrk INTEGER PRIMARY KEY, trip  INTEGER, trk  INTEGER);"
    ;

//================================

QString getStringField(const QSqlQuery& q, const QString& fieldName)
{
    int fieldNo = q.record().indexOf(fieldName);
    return q.value(fieldNo).toString();
}

double getDoubleField(const QSqlQuery& q, const QString& fieldName)
{
    int fieldNo = q.record().indexOf(fieldName);
    return q.value(fieldNo).toDouble();
}

QDateTime getDateTimeField(const QSqlQuery& q, const QString& fieldName)
{
    int fieldNo = q.record().indexOf(fieldName);
    return q.value(fieldNo).toDateTime();
}

int getIntField(const QSqlQuery& q, const QString& fieldName)
{
    int fieldNo = q.record().indexOf(fieldName);
    return q.value(fieldNo).toInt();
}

//================================

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

//================================

qmlRoute::qmlRoute()
    : _name("rte")
    , _rteid(0)
{
    ENTER("")
}

qmlRoute::qmlRoute(int id)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM routes WHERE rte='" + QString::number(id) + "'",db);
    if (q.next())
    {
        _rteid     = getIntField(q,"rte");
        _name      = getStringField(q,"name");
        _top       = getDoubleField(q,"top");
        _left      = getDoubleField(q,"left");
        _bottom    = getDoubleField(q,"bottom");
        _right     = getDoubleField(q,"right");
    }
    EXIT("")
}

qmlRoute::qmlRoute(const QSqlQuery& q)
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

//================================

qmlTrack::qmlTrack()
    : _name("trk")
    , _trkid(0)
{
    ENTER("")
}

qmlTrack::qmlTrack(int id)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM tracks WHERE trk='" + QString::number(id) + "'",db);
    if (q.next())
    {
        _trkid     = getIntField(q,"trk");
        _name      = getStringField(q,"name");
        _top       = getDoubleField(q,"top");
        _left      = getDoubleField(q,"left");
        _bottom    = getDoubleField(q,"bottom");
        _right     = getDoubleField(q,"right");
    }
    EXIT("")
}

qmlTrack::qmlTrack(const QSqlQuery& q)
{
    ENTER("")
    _trkid     = getIntField(q,"trk");
    _name      = getStringField(q,"name");
    _top       = getDoubleField(q,"top");
    _left      = getDoubleField(q,"left");
    _bottom    = getDoubleField(q,"bottom");
    _right     = getDoubleField(q,"right");
    EXIT("")
}

//================================

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

//================================

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
    _tripid = getIntField(q,"trip");
    _name = getStringField(q,"name");
    _triptime = getDateTimeField(q,"triptime");
    _tripdist = getDoubleField(q,"tripdist");
    _ascent = getDoubleField(q,"ascent");
    _descent = getDoubleField(q,"descent");
    _altmin = getDoubleField(q,"altmin");
    _altmax = getDoubleField(q,"altmax");
    _altavg = getDoubleField(q,"altavg");
    _speedmin = getDoubleField(q,"speedmin");
    _speedmax = getDoubleField(q,"speedmax");
    _speedavg = getDoubleField(q,"speedavg");
    EXIT("")
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

//================================

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

//================================

qmlMap::qmlMap()
    : _name("map")
    , _mapid(0)
{
    ENTER("")
}

qmlMap::qmlMap(int id)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM maps WHERE mapid='" + QString::number(id) + "'",db);
    if (q.next())
    {
        _mapid     = getIntField(q,"mapid");
        _name      = getStringField(q,"name");
        _top       = getDoubleField(q,"north");
        _left      = getDoubleField(q,"west");
        _bottom    = getDoubleField(q,"south");
        _right     = getDoubleField(q,"east");
    }
    EXIT("")
}

qmlMap::qmlMap(const QSqlQuery& q)
{
    ENTER("")
    _mapid     = getIntField(q,"mapid");
    _name      = getStringField(q,"name");
    _top       = getDoubleField(q,"north");
    _left      = getDoubleField(q,"west");
    _bottom    = getDoubleField(q,"south");
    _right     = getDoubleField(q,"east");
    EXIT("")
}

QDeclarativeListProperty<qmlRefpoint>
qmlMap::refpoints()
{
    ENTER("")
    return QDeclarativeListProperty<qmlRefpoint>(this, _refpts);
}

void
qmlMap::selectRefpoints(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"refpoints");
    r.clear();
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT mappt FROM mappoints WHERE mapid='" + QString::number(_mapid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlRefpoint(q.value(0).toInt())); }
    EXIT("")
}

//=================================

qmlDatabase::qmlDatabase()
    : _offset(0)
    , _limit(10)
{
    ENTER("")
    create();
    select();
    EXIT("")
}

QDeclarativeListProperty<qmlCategory>
qmlDatabase::categories()
{
    ENTER("")
    return QDeclarativeListProperty<qmlCategory>(this, _categories);
}

QDeclarativeListProperty<qmlTrip>
qmlDatabase::trips()
{
    ENTER("")
    return QDeclarativeListProperty<qmlTrip>(this, _trips);
}

QDeclarativeListProperty<qmlMap>
qmlDatabase::maps()
{
    ENTER("")
    return QDeclarativeListProperty<qmlMap>(this, _maps);
}

qmlCategory*
qmlDatabase::getCategory(int id)
{
    ENTER("")
    return new qmlCategory(id);
}

qmlTrip*
qmlDatabase::getTrip(int id)
{
    ENTER("")
    return new qmlTrip(id);
}

qmlMap*
qmlDatabase::getMap(int id)
{
    ENTER("")
    return new qmlMap(id);
}

qmlRefpoint*
qmlDatabase::getRefpoint(int id)
{
    ENTER("")
    return new qmlRefpoint(id);
}

qmlWaypoint*
qmlDatabase::getWaypoint(int id)
{
    ENTER("")
    return new qmlWaypoint(id);
}

qmlRoute*
qmlDatabase::getRoute(int id)
{
    ENTER("")
    return new qmlRoute(id);
}

qmlTrack*
qmlDatabase::getTrack(int id)
{
    ENTER("")
    return new qmlTrack(id);
}

void
qmlDatabase::setLimit(int value)
{
    ENTER("")
    _limit = value;
    select();
    emit limitChanged();
    EXIT("")
}

void
qmlDatabase::setOffset(int value)
{
    ENTER("")
    _offset = value;
    select();
    emit offsetChanged();
    EXIT("")
}

void
qmlDatabase::clear()
{
    ENTER("")
    for (int i=0; i<_categories.length(); i++) delete _categories[i];
    for (int i=0; i<_trips.length(); i++) delete _trips[i];
    _categories.clear();
    _trips.clear();
    EXIT("")
}

void
qmlDatabase::create()
{
    ENTER("")

    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);

    for (int i=0; i<dbsetup.length(); i++)
    {
        if (!q.exec(dbsetup[i]))
        { LOG("qmlDatabase::create() cmd: " << dbsetup[i] << " error: " << q.lastError() ) }
    }

    EXIT("")
}

void
qmlDatabase::select()
{
    ENTER("")

    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    clear();

    q.exec("SELECT * FROM trips LIMIT " + QString::number(_limit) + " OFFSET " + QString::number(_offset));
    while (q.next()) _trips.append(new qmlTrip(q));

    q.exec("SELECT * FROM categories LIMIT " + QString::number(_limit) + " OFFSET " + QString::number(_offset));
    while (q.next()) _categories.append(new qmlCategory(q));

    q.exec("SELECT * FROM maps LIMIT " + QString::number(_limit) + " OFFSET " + QString::number(_offset));
    while (q.next()) _maps.append(new qmlMap(q));

    EXIT("")
}

