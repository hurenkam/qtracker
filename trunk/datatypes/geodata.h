/*
 * geodata.h
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#ifndef GEODATA_H_
#define GEODATA_H_

#include <QString>
#include <QList>
#include <QMap>
#include "ui.h"

#ifdef Q_OS_SYMBIAN
#define MAPDIR      "/data/qtracker/maps/"
#define ROUTEDIR    "/data/qtracker/routes/"
#define TRACKDIR    "/data/qtracker/tracks/"
#define WAYPOINTDIR "/data/qtracker/"
#else
#define MAPDIR      "/Users/hurenkam/workspace/qtracker/maps/"
#define ROUTEDIR    "/Users/hurenkam/workspace/qtracker/routes/"
#define TRACKDIR    "/Users/hurenkam/workspace/qtracker/tracks/"
#define WAYPOINTDIR "/Users/hurenkam/workspace/qtracker/"
#endif

class WayPoint
{
protected:
	QString name;
	double latitude;
	double longitude;
	double elevation;
	QString time;
public:
	WayPoint(double lat=0, double lon=0, double ele=0.0, QString t="", QString n="wpt")
	    : latitude(lat), longitude(lon), elevation(ele), time(t), name(n) {}
	
	WayPoint(const WayPoint& w)
	    : latitude(w.Latitude())
	    , longitude(w.Longitude())
	    , elevation(w.Elevation())
	    , time(w.Time())
	    , name(w.Name()) {}

	QString Name() const               { return name; }
	double Latitude() const            { return latitude; }
	double Longitude() const           { return longitude; }
	double Elevation() const           { return elevation; }
	QString Time() const               { return time; }
	
	void SetName(QString n)            { name = n; }
	void SetLatitude(double l)         { latitude = l; }
	void SetLongitude(double l)        { longitude = l; }
	void SetElevation(double e)        { elevation = e; }
	void SetTime(QString t)            { time = t; }
	
	double distance(WayPoint *to);
	double bearing(WayPoint *to);
};

class WayPointList : public QObject
{
    Q_OBJECT
signals:
    void added(const QString&);
    void updated(const QString&);
    void removed(const QString&);
    
public:
    static WayPointList& Instance() { if (!instance) instance = new WayPointList(); return *instance; }
private:
    static WayPointList* instance;
    WayPointList() {};
protected:
	QMap<QString, WayPoint*> map;
	
public:
	// Todo: handle case if name already exists
	void AddWayPoint(WayPoint* w)       { map[w->Name()]=w; emit added(w->Name()); }
	void AddWayPoint(const WayPoint& w) { AddWayPoint(new WayPoint(w)); }
	QList<QString> WptNames()           { return map.keys(); }
	WayPoint& GetItem(QString n)        { return *map[n]; }
	QString FileName()                  { return QString(GetDrive() + QString(WAYPOINTDIR) + "waypoints.gpx"); }
};

class Bounds
{
protected:
	double minlatitude;
	double minlongitude;
	double maxlatitude;
	double maxlongitude;
public:
	Bounds(double minlat=0, double minlon=0, double maxlat=0, double maxlon=0)
		: minlatitude(minlat), minlongitude(minlon), maxlatitude(maxlat), maxlongitude(maxlon) {}
	double MinLatitude()  { return minlatitude; }
	double MinLongitude() { return minlongitude; }
	double MaxLatitude()  { return maxlatitude; }
	double MaxLongitude() { return maxlongitude; }
};

class Resolution
{
protected:
	int width;
	int height;
public:
	Resolution(int w=0, int h=0)
	    : width(w), height(h) {}
	void SetWidth(int w)  { width = w; }
	void SetHeight(int h) { height = h; }
	int Width()           { return width; }
	int Height()          { return height; }
};

class RefPoint: public WayPoint
{
protected:
	int x;
	int y;
public:
	RefPoint(int ax, int ay, double lat, double lon)
	    : WayPoint(lat,lon,0,"","ref"), x(ax), y(ay) {}
	
    int X() { return x; }
    int Y() { return y; }
    void SetX(int ax) { x = ax; }
    void SetY(int ay) { y = ay; }
};

class AreaMetaData
{
protected:
	QString name;
	Bounds bounds;
public:
	QString Name()                     { return name; }
	void SetName(QString n)            { name = n; }
	void SetBounds(Bounds b)           { bounds = b; }
};

class MapMetaData
{
protected:
	QString name;
	QString imagename;
	QList<RefPoint> refpoints;
	Resolution resolution;
	bool iscalibrated;
	double lat,lon, lon2x, lat2y, x2lon, y2lat;
	int x,y;
	
public:
	MapMetaData()
    : iscalibrated(false), lat(0), lon(0), lon2x(0), lat2y(0), x2lon(0), y2lat(0), x(0), y(0)
	{}
	
	QString Name()                     { return name; }
	void SetName(QString n)            { name = n; }
	void SetResolution(Resolution r)   { resolution = r;      Calibrate(); }
	bool AddRefPoint(RefPoint r)       { refpoints.append(r); Calibrate(); return true; }
    double Lon2x()                     { return lon2x; }
    double Lat2y()                     { return lat2y; }
    double X2Lon()                     { return x2lon; }
    double Y2Lat()                     { return y2lat; }

    void Calibrate();
    bool WgsArea(double& lat1, double& lon1, double& lat2, double& lon2);
    bool XY2Wgs(double ax, double ay, double& alat, double& alon);
    bool Wgs2XY(double alat, double alon, double& ax, double& ay);
    bool IsPositionOnMap(double alat, double alon);
    bool IsPositionOnMap(const WayPoint& w) { return IsPositionOnMap(w.Latitude(),w.Longitude()); }
    bool IsCalibrated() { return iscalibrated; }
    void SetSize(int w, int h) { resolution.SetWidth(w); resolution.SetHeight(h); }
    //void SetImageFilename(QString filename) { imagename = filename; };
    QString GetMetaFilename()  { return GetDrive() + QString(MAPDIR) + name + QString(".xml"); }
    QString GetImageFilename() { return GetDrive() + QString(MAPDIR) + name + QString(".jpg");}

private:
    void CalculateIndexesFromRefpoints(int i, int j);
    bool IsValidRefpointPair(int i, int j);
};

class Track: public QObject
{
    Q_OBJECT
signals:
    void updated(WayPoint& w);
protected:
	QString name;
	QList<WayPoint*> list;
	//int updatetime;
	//int updatedistance;
public:
	//Track(QString n="", int t=0, int d=0): name(n), updatetime(t), updatedistance(d) {}
	QString Name() const             { return name; }
	QString FileName() const         { return QString(GetDrive() + QString(TRACKDIR) + name + ".gpx"); }
	void SetName(QString n)          { name = n; }
	void AddPoint(WayPoint* w)       { list.append(w); emit updated(*w); }
	WayPoint& GetItem(int i) const   { return *list[i]; }
	int Length() const               { return list.length(); }
};

class TrackList: public QObject
{
    Q_OBJECT
signals:
    void added(Track*);
    void added(const QString&);
    void removed(QString);
    
public:
    static TrackList* Instance() { if (!instance) instance = new TrackList(); return instance; }
private:
    static TrackList* instance;
    TrackList() {};

protected:
	QMap<QString, Track*> map;
	
public:
	// Todo: handle case if name already exists
	void AddTrack(Track* t)                { map[t->Name()]=t; emit added(t); emit added(t->Name()); }
	void RemoveTrack(QString name);
	void AddMetaData(AreaMetaData* m)      { }
	QList<QString> Keys()                  { return map.keys(); }
	Track& GetItem(QString k) const        { return *map[k]; }
};

class Route: public QObject
{
    Q_OBJECT
signals:
    void updated(WayPoint& w);
protected:
	QString name;
	QList<WayPoint*> list;
public:
	const QString Name() const        { return name; }
	const QString FileName() const    { return QString(GetDrive() + QString(TRACKDIR) + name + ".gpx"); }
	void SetName(QString n)           { name = n; }
	void AddPoint(WayPoint* w)        { list.append(w); emit updated(*w); }
};

class RouteList: public QObject
{
    Q_OBJECT
signals:
    void updated(QString n);
public:
    static RouteList* Instance() { if (!instance) instance = new RouteList(); return instance; }
private:
    static RouteList* instance;
    RouteList() {};

protected:
	QMap<QString, Route*> map;
	
public:
	// Todo: handle case if name already exists
	void AddRoute(Route* r)            { map[r->Name()]=r; emit updated(r->Name()); }
	void AddMetaData(AreaMetaData* m)  { }
};

class MapList
{
public:
    static MapList& Instance() { if (!instance) new MapList(); return *instance; }
private:
    static MapList* instance;
    MapList();

protected:
	QMap<QString, MapMetaData*> map;
	
public:
	MapMetaData& NewMetaData(QString n)    { map[n] = new MapMetaData(); map[n]->SetName(n); return *map[n]; }
	void AddMetaData(MapMetaData* m)       { map[m->Name()]=m; }
	QList<QString> MapNames()              { return map.keys(); }
	MapMetaData& GetItem(QString n)        { return *map[n]; }
	QStringList FindMapsForPosition(const WayPoint& w);
};

#endif /* GEODATA_H_ */
