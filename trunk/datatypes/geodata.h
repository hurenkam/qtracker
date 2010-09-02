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
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include "ui.h"

#ifdef Q_OS_SYMBIAN
#define MAPDIR      "/data/qtracker/maps/"
#define ROUTEDIR    "/data/qtracker/routes/"
#define TRACKDIR    "/data/qtracker/tracks/"
#define WAYPOINTDIR "/data/qtracker/"
#else
#define MAPDIR      "/home/hurenkam/Workspace/qtracker/maps/"
#define ROUTEDIR    "/home/hurenkam/Workspace/qtracker/routes/"
#define TRACKDIR    "/home/hurenkam/Workspace/qtracker/tracks/"
#define WAYPOINTDIR "/home/hurenkam/Workspace/qtracker/"
#endif

using namespace QtMobility;

class WayPoint;

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
	bool Contains(const WayPoint& w);
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

namespace geodata
{	
    static const float pi = 3.14159265358979323846f;

    extern QString UniqueName(QString prefix);
	
	extern QString GeoTime();

	extern void CalculateDistanceAndBearing(
		double fromlat,
		double fromlon,
		double tolat,
		double tolon,
		double &distance,
		double &bearing );
	
	extern bool IsValueInRange(double v, double r1, double r2);
/*	
    enum Datum
	{
        Wgs84_Geo = 0,
        Wgs84_DMS,
        UTMUPS,
        MGRS,
        RD
	};
*/
}

#endif /* GEODATA_H_ */
