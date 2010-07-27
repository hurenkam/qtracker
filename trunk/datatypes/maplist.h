/*
 * geodata.h
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#ifndef MAPLIST_H_
#define MAPLIST_H_

#include <QString>
#include <QList>
#include <QMap>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include "ui.h"
#include "waypointlist.h"

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
	int Width() const     { return width; }
	int Height() const    { return height; }
};

class RefPoint: public WayPoint
{
protected:
	int x;
	int y;
public:
	RefPoint(const RefPoint& r)
	    : WayPoint(r), x(r.X()), y(r.Y()) {}
	
	RefPoint(int ax, int ay, double lat, double lon)
	    : WayPoint(lat,lon,0,"","ref"), x(ax), y(ay) {}
	
    int X() const     { return x; }
    int Y() const     { return y; }
    void SetX(int ax) { x = ax; }
    void SetY(int ay) { y = ay; }
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
	
	QString Name() const               { return name; }
	void SetName(QString n)            { name = n; }
	void SetResolution(Resolution r)   { resolution = r;      Calibrate(); }
	Resolution GetResolution() const   { return resolution; }
	int Length() const                 { return refpoints.length(); }
	RefPoint GetRefPoint(int i) const  { return refpoints[i]; }
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
    QString GetMetaFilename() const  { return GetDrive() + QString(MAPDIR) + name + QString(".xml"); }
    QString GetImageFilename() const { return GetDrive() + QString(MAPDIR) + name + QString(".jpg");}

private:
    void CalculateIndexesFromRefpoints(int i, int j);
    bool IsValidRefpointPair(int i, int j);
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

#endif /* MAPLIST_H_ */
