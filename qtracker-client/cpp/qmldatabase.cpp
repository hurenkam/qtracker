#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include <QTimer>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

const QStringList dbsetup = QStringList()
    << "CREATE TABLE IF NOT EXISTS maps          (mapid   INTEGER PRIMARY KEY, name  TEXT,    time TEXT, filename TEXT,    north     NUMBER, east      NUMBER, south    NUMBER, west NUMBER);"
    << "CREATE TABLE IF NOT EXISTS mappoints     (mappt   INTEGER PRIMARY KEY, mapid INTEGER, name TEXT, latitude NUMBER,  longitude NUMBER, x         NUMBER, y        NUMBER);"
    << "CREATE TABLE IF NOT EXISTS waypoints     (wpt     INTEGER PRIMARY KEY, name  TEXT,    time TEXT, latitude NUMBER,  longitude NUMBER, altitude  NUMBER, notes    TEXT);"
    << "CREATE TABLE IF NOT EXISTS routes        (rte     INTEGER PRIMARY KEY, name  TEXT,    time TEXT, north    NUMBER,  west      NUMBER, south     NUMBER, east     NUMBER);"
    << "CREATE TABLE IF NOT EXISTS routepoints   (rtept   INTEGER PRIMARY KEY, rte   INTEGER, wpt  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS tracks        (trk     INTEGER PRIMARY KEY, name  TEXT,    time TEXT, interval INTEGER, north     NUMBER, west      NUMBER, south    NUMBER, east NUMBER);"
    << "CREATE TABLE IF NOT EXISTS trackpoints   (trkpt   INTEGER PRIMARY KEY, trk   INTEGER, time TEXT, latitude NUMBER,  longitude NUMBER, altitude  NUMBER);"
    << "CREATE TABLE IF NOT EXISTS categories    (cat     INTEGER PRIMARY KEY, name  TEXT,    time TEXT, notes    TEXT);"
    << "CREATE TABLE IF NOT EXISTS catwaypoints  (catwpt  INTEGER PRIMARY KEY, cat   INTEGER, wpt  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS catroutes     (catrte  INTEGER PRIMARY KEY, cat   INTEGER, rte  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS cattracks     (cattrk  INTEGER PRIMARY KEY, cat   INTEGER, trk  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS trips         (trip    INTEGER PRIMARY KEY, name  TEXT,    time TEXT, ascent   NUMBER,  descent   NUMBER, altmin    NUMBER, altmax   NUMBER, altavg NUMBER speedmin NUMBER, speedmax NUMBER, speedavg NUMBER, triptime DATETIME, tripdist NUMBER);"
    << "CREATE TABLE IF NOT EXISTS tripwaypoints (tripwpt INTEGER PRIMARY KEY, trip  INTEGER, wpt  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS triproutes    (triprte INTEGER PRIMARY KEY, trip  INTEGER, rte  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS triptracks    (triptrk INTEGER PRIMARY KEY, trip  INTEGER, trk  INTEGER);"
    << "CREATE TABLE IF NOT EXISTS settings      (setting TEXT UNIQUE,         value TEXT);"
    ;

//================================

QVariant getVariantField(const QSqlQuery& q, const QString& fieldName)
{
    int fieldNo = q.record().indexOf(fieldName);
    return q.value(fieldNo);
}

QString getStringField(const QSqlQuery& q, const QString& fieldName)
{
    return getVariantField(q,fieldName).toString();
}

double getDoubleField(const QSqlQuery& q, const QString& fieldName)
{
    return getVariantField(q,fieldName).toDouble();
}

int getIntField(const QSqlQuery& q, const QString& fieldName)
{
    return getVariantField(q,fieldName).toInt();
}

QDateTime getDateTimeField(const QSqlQuery& q, const QString& fieldName)
{
    return getVariantField(q,fieldName).toDateTime();
}

//================================

qmlDatabasePrivate::qmlDatabasePrivate()
    : _limit(50)
    , _offset(0)
{
    ENTER("")
    create();
    select();
    readSettings();
    _timer.setSingleShot(true);
    _timer.setInterval(5000);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(saveSettings()));
    EXIT("")
}

void
qmlDatabasePrivate::setLimit(int value)
{
    ENTER("")
    _limit = value;
    select();
    emit limitChanged();
}

void
qmlDatabasePrivate::setOffset(int value)
{
    ENTER("")
    _offset = value;
    select();
    emit offsetChanged();
}

void
qmlDatabasePrivate::clear()
{
    ENTER("")
    for (int i=0; i<_categories.length(); i++) delete _categories[i];
    for (int i=0; i<_trips.length(); i++) delete _trips[i];
    _categories.clear();
    _trips.clear();
    EXIT("")
}

void
qmlDatabasePrivate::create()
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
qmlDatabasePrivate::select()
{
    ENTER("")

    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    clear();

    q.exec("SELECT * FROM trips ORDER BY trip DESC LIMIT " + QString::number(_limit) + " OFFSET " + QString::number(_offset));
    while (q.next()) _trips.append(new qmlTrip(q));

    q.exec("SELECT * FROM categories ORDER BY name LIMIT " + QString::number(_limit) + " OFFSET " + QString::number(_offset));
    while (q.next()) _categories.append(new qmlCategory(q));

    q.exec("SELECT * FROM maps ORDER BY name LIMIT " + QString::number(_limit) + " OFFSET " + QString::number(_offset));
    while (q.next()) _maps.append(new qmlMap(q));

    EXIT("")
}

void qmlDatabasePrivate::readSettings()
{
    ENTER("")

    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);

    q.exec("SELECT setting, value FROM settings");
    while (q.next())
    {
        QString  key   = getStringField(q,"setting");
        QVariant value = getVariantField(q,"value");
        _settings[key] = value;
        //LOG("found setting: " << key << " = " << value);
    }

    EXIT("")
}

void qmlDatabasePrivate::saveSettings()
{
    ENTER("")

    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);

    for (int i=0; i < _dirtysettings.count(); i++)
    {
        q.exec("INSERT OR REPLACE INTO settings (setting,value) VALUES (\"" + _dirtysettings[i] + "\",\"" + _settings[_dirtysettings[i]].toString() + "\")");
    }
    _dirtysettings.clear();

    EXIT("")
}

void qmlDatabasePrivate::set(const QString& key, const QVariant& value)
{
    _settings[key]=value;
    if (!_dirtysettings.contains(key))
    { _dirtysettings.append(key); }
    _timer.start();
    emit settingChanged(key,value);
}

QVariant qmlDatabasePrivate::get(const QString& key, const QVariant& defaultValue)
{
    if (_settings.contains(key))
        return _settings[key];

    set(key,defaultValue);
    return defaultValue;
}

//================================

qmlDatabasePrivate* qmlDatabase::p = 0;

qmlDatabase::qmlDatabase()
{
    ENTER("")
    if (!p) p = new qmlDatabasePrivate();
    connect(p, SIGNAL(offsetChanged()), this, SLOT(offsetChanged()));
    connect(p, SIGNAL(limitChanged()), this, SLOT(limitChanged()));
    connect(p, SIGNAL(settingChanged(QString, QVariant)), this, SLOT(settingChanged(QString,QVariant)));
    EXIT("")
}

QDeclarativeListProperty<qmlCategory>
qmlDatabase::categories()
{
    ENTER("")
    return QDeclarativeListProperty<qmlCategory>(this, p->_categories);
}

QDeclarativeListProperty<qmlTrip>
qmlDatabase::trips()
{
    ENTER("")
    return QDeclarativeListProperty<qmlTrip>(this, p->_trips);
}

QDeclarativeListProperty<qmlMap>
qmlDatabase::maps()
{
    ENTER("")
    return QDeclarativeListProperty<qmlMap>(this, p->_maps);
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
    p->setLimit(value);
}

int
qmlDatabase::limit()
{ return p->_limit; }

void
qmlDatabase::setOffset(int value)
{
    ENTER("")
    p->setOffset(value);
}

int
qmlDatabase::offset()
{ return p->_offset; }

void qmlDatabase::set(const QString& key, const QVariant& value)
{ p->set(key,value); }

QVariant qmlDatabase::get(const QString& key, const QVariant& defaultValue)
{ return p->get(key,defaultValue); }
