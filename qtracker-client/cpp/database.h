#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QPoint>
#include <QGeoCoordinate>
#include <QGeoBoundingBox>
#include <QSqlDatabase>
#include <QString>
#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QMap>
#include <QUrl>

using namespace QtMobility;

class Waypoint : public QGeoCoordinate
{
public:
    explicit Waypoint(double latitude, double longitude, double elevation=0.0)
        : QGeoCoordinate(latitude,longitude,elevation)
        , name("")
    {}

    explicit Waypoint(QString wptname, double latitude, double longitude, double elevation=0.0)
        : QGeoCoordinate(latitude,longitude,elevation)
        , name(wptname)
    {}

    QString getName()                    { return name; }
    void setName      (QString v)        { name = v; }

private:
    QString name;
};

class Mappoint
{
public:
    explicit Mappoint() {}
    explicit Mappoint(double latitude, double longitude, int x, int y)
        : _map(x,y)
        , _geo(latitude,longitude)
    {}

    explicit Mappoint(const QGeoCoordinate& geo, const QPoint& map)
        : _map(map)
        , _geo(geo)
    {}

    int x()              { return _map.x(); }
    int y()              { return _map.y(); }
    double latitude()    { return _geo.latitude();  }
    double longitude()   { return _geo.longitude(); }
    QPoint map()         { return _map; }
    QGeoCoordinate geo() { return _geo; }

private:
    QPoint         _map;
    QGeoCoordinate _geo;
};

class Area : public QGeoBoundingBox
{
public:
    explicit Area(Waypoint topleft, Waypoint bottomright) : QGeoBoundingBox(topleft,bottomright) {}

signals:

public slots:

};

class WaypointList: public QList<Waypoint>
{
public:
    explicit WaypointList() {}
};

class MappointList: public QList<Mappoint>
{
public:
    explicit MappointList() {}
};

class Map
{
public:
    explicit Map(QString name, QString filename, Area area, int id=-1)
        : _name(name)
        , _filename(filename)
        , _area(area)
        , _id(id)
        , _iscalibrated(false)
    {}

    QString  name()              { return _name; }
    Area     area()              { return _area; }
    int      id()                { return _id; }
    QUrl     url()               { return _filename; }
    bool     calibrate();
    bool     isCalibrated()      { return _iscalibrated; }
    double   mapy2lat(double v)  { return _iscalibrated? _baselat + _dlat/_dy   * (v - _basey) : 0; }
    double   mapx2lon(double v)  { return _iscalibrated? _baselon + _dlon/_dx   * (v - _basex) : 0; }
    double   lat2mapy(double v)  { return _iscalibrated? _basey   + _dy/_dlat * (v - _baselat) : 0; }
    double   lon2mapx(double v)  { return _iscalibrated? _basex   + _dx/_dlon * (v - _baselon) : 0; }

private:
    QString  _name;
    QUrl     _filename;
    Area     _area;
    int      _id;
    double   _baselat;
    double   _baselon;
    double   _basex;
    double   _basey;
    double   _dx;
    double   _dy;
    double   _dlat;
    double   _dlon;
    bool     _iscalibrated;
};

class MapList: public QList<Map>
{
public:
    explicit MapList() {}
};

class Database: public QObject
{
    Q_OBJECT

public:
    static Database& Instance();
    static QSqlDatabase& Db() { return Instance().db; }
    static int Platform() { return Instance().platform; }

    WaypointList Waypoints(Area area);
    MapList      Maps     (QGeoCoordinate geo=QGeoCoordinate());
    Map*         GetMap   (int mapid);
    MappointList Mappoints(int mapid);

private:
    explicit Database();

signals:

public slots:

private:
    static Database *instance;
    QSqlDatabase db;
    int platform;
};

#endif // DATABASE_H
