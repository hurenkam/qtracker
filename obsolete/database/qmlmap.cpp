#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "qmldatabase.h"

//#define ENABLE_DEBUG
#include "helpers.h"

qmlMap::qmlMap()
    : _name("map")
    , _mapid(-1)
    , _iscalibrated(false)
{
    ENTER("")
}

qmlMap::qmlMap(int id)
    : _name("map")
    , _mapid(-1)
{
    ENTER("")
    setMapid(id);
    EXIT("")
}

qmlMap::qmlMap(const QSqlQuery& q)
    : _name("map")
    , _mapid(-1)
{
    ENTER("")
    load(q);
    EXIT("")
}

void qmlMap::setMapid(int value)
{
    ENTER("")
    if (value>0)
    {
        QSqlDatabase& db = qmlDatabase::Db();
        QSqlQuery q("SELECT * FROM maps WHERE mapid='" + QString::number(value) + "'",db);
        if (q.next())
            load(q);
        selectRefpoints(0,10);
        calibrate();
    }
    else
    {
        _mapid = value;
    }
    EXIT("")
}

void
qmlMap::load(const QSqlQuery& q)
{
    ENTER("")
    _mapid     = getIntField(q,"mapid");
    _name      = getStringField(q,"name");
    _filename  = getStringField(q,"filename");
    _top       = getDoubleField(q,"north");
    _left      = getDoubleField(q,"west");
    _bottom    = getDoubleField(q,"south");
    _right     = getDoubleField(q,"east");
    EXIT("")
}

void qmlMap::save()
{
    ENTER("")
    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q(db);
    if (_mapid>0)
    {
        q.exec("REPLACE INTO maps (mapid,name,filename,north,west,south,east) VALUES (\"" + QString::number(_mapid) + "\",\"" + _name  + "\",\"" + _filename  + "\",\"" + QString::number(_top)  + "\",\"" + QString::number(_left)  + "\",\"" + QString::number(_bottom)  + "\",\"" + QString::number(_right) + "\")");
    }
    else
    {
        q.exec("INSERT  INTO maps       (name,filename,north,west,south,east) VALUES                                    (\""  + _name  + "\",\"" + _filename  + "\",\"" + QString::number(_top)  + "\",\"" + QString::number(_left)  + "\",\"" + QString::number(_bottom)  + "\",\"" + QString::number(_right) + "\")");
        _mapid = q.lastInsertId().toInt();
        emit mapidChanged();
    }
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
    QSqlDatabase& db = qmlDatabase::Db();
    QSqlQuery q("SELECT mappt FROM mappoints WHERE mapid='" + QString::number(_mapid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlRefpoint(q.value(0).toInt())); }
    EXIT("")
}

bool qmlMap::calibrate()
{
    LOG("qmlMap::calibrate()")
    _iscalibrated = false;
    if (_refpts.count()>1)
    {
        qmlRefpoint* p1 = _refpts[0];
        qmlRefpoint* p2 = _refpts[1];
        _baselat = p1->latitude();
        _baselon = p1->longitude();
        _basex = p1->x();
        _basey = p1->y();
        _dlon = p2->longitude()-p1->longitude();
        _dlat = p2->latitude()-p1->latitude();
        _dx = p2->x()-p1->x();
        _dy = p2->y()-p1->y();
        if (!_iscalibrated)
        {
            _iscalibrated = true;
            emit calibrationChanged();
        }
        LOG("qmlMap::calibrate(): " << _baselat << ":" << _dlat << ":" << mapy2lat(p2->y())         << " " << _baselon << ":" << _dlon << " " << mapx2lon(p2->y()))
        LOG("qmlMap::calibrate(): " << _basex   << ":" << _dx   << ":" << lon2mapx(p2->longitude()) << " " << _basey   << ":" << _dy   << " " << lat2mapy(p2->latitude()))
    }
    else
    {
        if (_iscalibrated)
        {
            _iscalibrated = false;
            emit calibrationChanged();
        }
    }
    return _iscalibrated;
}
