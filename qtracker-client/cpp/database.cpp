#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include "database.h"

#define ENABLE_DEBUG
#include "helpers.h"

Database* Database::instance = 0;

Database::Database()
{
    ENTER("")

    db = QSqlDatabase::addDatabase("QSQLITE");

    #if defined(Q_OS_SYMBIAN)
    platform = 0;
    #elif defined(Q_WS_MAEMO_5)
    platform = 1;
    #elif defined(Q_WS_SIMULATOR)
    platform = 2;
    #else
    platform = -1;
    #endif

    switch (platform)
    {
    case 0:  db.setDatabaseName("e:\\data\\qtracker\\database.sqlite"); break;
    case 1:  db.setDatabaseName("database.sqlite"); break;
    case 2:  db.setDatabaseName("c:\\data\\qtracker\\database.sqlite"); break;
    default: db.setDatabaseName("database.sqlite"); break;
    }

    bool result = db.open();
    if (!result)
    {
        EXIT("unable to open db" << db.lastError().text())
    }
    else
    {
        EXIT("")
    }
}

Database& Database::Instance()
{
    //ENTER("")

    if (!instance)
        instance = new Database();

    return *instance;
}

WaypointList Database::Waypoints(Area area)
{
    LOG("Database::getWaypoints()" << area.topLeft().latitude() << ", " << area.topLeft().longitude() << ", " << area.bottomRight().latitude() << ", " << area.bottomRight().longitude())

    WaypointList result;

    QString s = "SELECT * FROM waypoints WHERE longitude>=:left AND longitude<=:right AND latitude>=:bottom AND latitude<=:top";
    QSqlQuery q(db);
    q.prepare(s);
    q.bindValue(":left",   area.topLeft().longitude());
    q.bindValue(":right",  area.bottomRight().longitude());
    q.bindValue(":top",    area.topLeft().latitude());
    q.bindValue(":bottom", area.bottomRight().latitude());
    q.exec();

    QSqlRecord r = q.record();

    int nameCol = r.indexOf("name");
    int latCol = r.indexOf("latitude");
    int lonCol = r.indexOf("longitude");
    while (q.next())
    {
        LOG("Database::getWaypoints() found waypoint: " << q.value(nameCol).toString())
        result.append(Waypoint(q.value(nameCol).toString(),q.value(latCol).toDouble(),q.value(lonCol).toDouble()));
    }

    return result;
}

MapList Database::Maps(QGeoCoordinate geo)
{
    QSqlQuery q(db);
    if (geo.isValid())
    {
        LOG("Database::getMaps()" << geo.latitude() << ", " << geo.longitude())
        QString s = "SELECT * FROM maps WHERE north<=:lat AND south>=:lat AND west<=:lon AND east>=:lon";
        q.prepare(s);
        q.bindValue(":lon", geo.longitude());
        q.bindValue(":lat", geo.latitude());
        q.exec();
    }
    else
    {
        LOG("Database::getMaps()")
        QString s = "SELECT * FROM maps";
        q.prepare(s);
        q.exec();
    }

    MapList result;
    int mapid=-1;
    QSqlRecord r = q.record();

    int idCol    = r.indexOf("mapid");
    int nameCol  = r.indexOf("name");
    int northCol = r.indexOf("north");
    int eastCol  = r.indexOf("east");
    int southCol = r.indexOf("south");
    int westCol  = r.indexOf("west");
    int fileCol  = r.indexOf("filename");
    while (q.next())
    {
        mapid = q.value(idCol).toInt();
        LOG("Database::getMaps() found map: " << mapid << " " << q.value(nameCol).toString())
        result[mapid] =
            Map(q.value(nameCol).toString(),
                q.value(fileCol).toString(),
                Area(
                    Waypoint(q.value(northCol).toDouble(),q.value(westCol).toDouble()),
                    Waypoint(q.value(southCol).toDouble(),q.value(eastCol).toDouble())
                ),
                mapid
            );
    }

    return result;
}

Map* Database::GetMap(int mapid)
{
    Map* result = 0;

    QSqlQuery q(db);
    LOG("Database::getMap(" << mapid << ")")
    QString s = "SELECT * FROM maps WHERE mapid = "+QString::number(mapid);
    //q.bindValue(":id", mapid);
    q.prepare(s);
    if (!q.exec())
    {
        LOG("Database::getMap(): query failed to execure. " << q.lastError());
        return result;
    }

    QSqlRecord r = q.record();
    int idCol    = r.indexOf("mapid");
    int nameCol  = r.indexOf("name");
    int northCol = r.indexOf("north");
    int eastCol  = r.indexOf("east");
    int southCol = r.indexOf("south");
    int westCol  = r.indexOf("west");
    int fileCol  = r.indexOf("filename");
    //while (!result && q.next())
    if (!q.next())
    {
        LOG("Database::getMap(): query did not produce any results. " << q.lastError());
        return result;
    }

    mapid = q.value(idCol).toInt();
    LOG("Database::getMap() found map: " << mapid << " " << q.value(nameCol).toString())
    result = new Map(q.value(nameCol).toString(),
            q.value(fileCol).toString(),
            Area(
                Waypoint(q.value(northCol).toDouble(),q.value(westCol).toDouble()),
                Waypoint(q.value(southCol).toDouble(),q.value(eastCol).toDouble())
            ),
            mapid
        );

    return result;
}

MappointList Database::Mappoints(int mapid)
{
    MappointList result;

    QSqlQuery q(db);
    LOG("Database::getMap(" << mapid << ")")
    QString s = "SELECT * FROM mappoints WHERE mapid = "+QString::number(mapid);
    q.prepare(s);
    q.exec();

    QSqlRecord r = q.record();
    int mapptCol = r.indexOf("mappt");
    int mapidCol = r.indexOf("mapid");
    int latCol   = r.indexOf("latitude");
    int lonCol   = r.indexOf("longitude");
    int xCol     = r.indexOf("x");
    int yCol     = r.indexOf("y");
    while (q.next())
    {
        double lat = q.value(latCol).toDouble();
        double lon = q.value(lonCol).toDouble();
        int    x   = q.value(xCol).toInt();
        int    y   = q.value(yCol).toInt();
        LOG("Database::getMappoints() found mappoint: (" << x << "," << y << ")-(" << lat << "," << lon << ")")
        result.append(Mappoint(lat,lon,x,y));
    }

    return result;
}

bool Map::calibrate()
{
    LOG("Map::calibrate()")
    _iscalibrated = false;
    MappointList points = Database::Instance().Mappoints(_id);
    if (points.count()>1)
    {
        Mappoint p1 = points[0];
        Mappoint p2 = points[1];
        _baselat = p1.latitude();
        _baselon = p1.longitude();
        _basex = p1.x();
        _basey = p1.y();
        //_dx   = (p2.longitude()-p1.longitude())/(p2.x()-p1.x());
        //_dy   = (p2.latitude()-p1.latitude())/(p2.y()-p1.y());
        //_dlon = (p2.x()-p1.x())/(p2.longitude()-p1.longitude());
        //_dlat = (p2.y()-p1.y())/(p2.latitude()-p1.latitude());
        _dlon = p2.longitude()-p1.longitude();
        _dlat = p2.latitude()-p1.latitude();
        _dx = p2.x()-p1.x();
        _dy = p2.y()-p1.y();
        _iscalibrated = true;
        LOG("Map::calibrate(): " << _baselat << ":" << _dlat << ":" << mapy2lat(p2.y())         << " " << _baselon << ":" << _dlon << " " << mapx2lon(p2.y()))
        LOG("Map::calibrate(): " << _basex   << ":" << _dx   << ":" << lon2mapx(p2.longitude()) << " " << _basey   << ":" << _dy   << " " << lat2mapy(p2.latitude()))
    }
    return _iscalibrated;
}

//double   mapy2lat(double v)  { return _iscalibrated? _baselat + _dlat * (v - _basey)   : 0; }
//double   mapx2lon(double v)  { return _iscalibrated? _baselon + _dlon * (v - _basex)   : 0; }
//double   lat2mapy(double v)  { return _iscalibrated? _basey   + _dy   * (v - _baselat) : 0; }
//double   lon2mapx(double v)  { return _iscalibrated? _basex   + _dx   * (v - _baselon) : 0; }
