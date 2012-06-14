/*
 * geodata.cpp
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#include "geodata.h"
#include "gpxio.h"
#include "ui.h"
#include "waypointlist.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

bool Bounds::Contains(const WayPoint& w)
{
	if (w.Latitude()  < minlatitude)  return false;
	if (w.Latitude()  > maxlatitude)  return false;
	if (w.Longitude() < minlongitude) return false;
	if (w.Longitude() > maxlongitude) return false;
	return true;
}

namespace geodata
{
	QString UniqueName(QString prefix)
	{
		QDateTime curtime = QDateTime::currentDateTime().toUTC();
		return prefix + curtime.toString("-yyyyMMdd-hhmmss");
	}
	
	QString GeoTime()
	{
		QDateTime curtime = QDateTime::currentDateTime().toUTC();
		return curtime.toString("yyyy-MM-ddThh:mm:ssZ");
	}
	
	void CalculateDistanceAndBearing(
		double fromlat,
		double fromlon,
		double tolat,
		double tolon,
		double &distance,
		double &bearing )
	{
		double earths_radius = (6378137.0 + 6356752.3141) / 2.0;
	
		double from_theta = float(fromlat) / 360.0 * 2.0 * pi;
		double from_landa = float(fromlon) / 360.0 * 2.0 * pi;
		double to_theta = float(tolat)     / 360.0 * 2.0 * pi;
		double to_landa = float(tolon)     / 360.0 * 2.0 * pi;
	
		distance = acos(
				sin(from_theta) * sin(to_theta) +
				cos(from_theta) * cos(to_theta) * cos(to_landa-from_landa)
			) * earths_radius;
	
		bearing = atan2(
				sin(to_landa-from_landa) * cos(to_theta),
				cos(from_theta) * sin(to_theta) -
				sin(from_theta) * cos(to_theta) * cos(to_landa-from_landa)
			);
		bearing = bearing / 2.0 / pi * 360.0;
	}
	
	bool IsValueInRange(double v, double r1, double r2)
	{
		if (r1 > r2)
			if ((v < r2) || (v > r1))
				return false;
	
		if (r2 > r1)
			if ((v < r1) || (v > r2))
				return false;
	
		return true;
	}
}
