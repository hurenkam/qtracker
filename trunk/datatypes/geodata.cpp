/*
 * geodata.cpp
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#include "geodata.h"
#include "gpxio.h"
#include "ui.h"

WayPointList* WayPointList::instance = 0;
RouteList*    RouteList::instance = 0;
TrackList*    TrackList::instance = 0;
MapList*      MapList::instance = 0;

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

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

const float PI = 3.14159265358979323846f;

static void CalculateDistanceAndBearing(
    double fromlat,
    double fromlon,
    double tolat,
    double tolon,
    double &distance,
    double &bearing )
{
    double earths_radius = (6378137.0 + 6356752.3141) / 2.0;

    double from_theta = float(fromlat) / 360.0 * 2.0 * PI;
    double from_landa = float(fromlon) / 360.0 * 2.0 * PI;
    double to_theta = float(tolat)     / 360.0 * 2.0 * PI;
    double to_landa = float(tolon)     / 360.0 * 2.0 * PI;

    distance = acos(
            sin(from_theta) * sin(to_theta) +
            cos(from_theta) * cos(to_theta) * cos(to_landa-from_landa)
        ) * earths_radius;

    bearing = atan2(
            sin(to_landa-from_landa) * cos(to_theta),
            cos(from_theta) * sin(to_theta) -
            sin(from_theta) * cos(to_theta) * cos(to_landa-from_landa)
        );
    bearing = bearing / 2.0 / PI * 360.0;
}

double WayPoint::distance(WayPoint* to)
{
	double d, b;
	CalculateDistanceAndBearing(Latitude(),Longitude(),to->Latitude(),to->Longitude(),d,b);
	return d;
}

double WayPoint::bearing(WayPoint* to)
{
	double d, b;
	CalculateDistanceAndBearing(Latitude(),Longitude(),to->Latitude(),to->Longitude(),d,b);
	return b;
}

static bool IsValueInRange(double v, double r1, double r2)
{
    if (r1 > r2)
        if ((v < r2) || (v > r1))
            return false;

    if (r2 > r1)
        if ((v < r1) || (v > r2))
            return false;

    return true;
}

WayPointList::WayPointList()
: settings("karpeer.net","qTracker",this) 
{ 
	instance = this;
	int length = settings.beginReadArray("wpt/list");
	for (int i=0; i<length; i++)
	{
		settings.setArrayIndex(i);
		WayPoint* w = new WayPoint();
		w->SetName      (settings.value("name").toString());
		w->SetLatitude  (settings.value("latitude").toDouble());
		w->SetLongitude (settings.value("longitude").toDouble());
		w->SetElevation (settings.value("elevation").toDouble());
		w->SetTime      (settings.value("time").toString());
		AddWayPoint(w);
		if (settings.value("visible","false").toBool())
			visiblekeys.append(w->Name());
	}
	settings.endArray();
};

void WayPointList::SaveSettings()
{ 
	QStringList names = Keys();
	settings.beginWriteArray("wpt/list",names.length());
	for (int i=0; i<names.length(); i++)
	{
		const WayPoint& w = GetItem(names[i]);
		settings.setArrayIndex(i);
		settings.setValue("name",      w.Name());
		settings.setValue("latitude",  w.Latitude());
		settings.setValue("longitude", w.Longitude());
		settings.setValue("elevation", w.Elevation());
		settings.setValue("time",      w.Time());
		if (visiblekeys.contains(names[i]))
			settings.setValue("visible", true);
		else
			settings.setValue("visible", false);
	}
	settings.endArray();
	settings.sync();
}

TrackList::TrackList()
: settings("karpeer.net","qTracker",this) 
{ 
	instance = this;
	int length = settings.beginReadArray("trk/list");
	for (int i=0; i<length; i++)
	{
		settings.setArrayIndex(i);
	    QString filename = GetDrive() + TRACKDIR + settings.value("name").toString() + ".gpx";
	    LOG( "TrackList::TrackList():" << filename; )
	    GpxIO::Instance()->ImportGpxFile(filename);
	}
	settings.endArray();
};

void TrackList::SaveSettings()
{
	QStringList names = Keys();
	settings.beginWriteArray("trk/list",names.length());
	for (int i=0; i<names.length(); i++)
	{
		settings.setArrayIndex(i);
		settings.setValue("name",names[i]);
	}
	settings.endArray();
	settings.sync();
} 

void TrackList::RemoveTrack(QString name)         
{ 
    LOG( "TrackList::RemoveTrack()"; )
	Track* t = map[name]; 
	map.remove(name); 
	delete t; 
    emit removed(name); 
}


MapList::MapList()
{
    LOG( "MapList::MapList()"; )
	instance = this;
    QStringList files;

    QDir directory = QDir(GetDrive() + QString(MAPDIR));

    files = directory.entryList(QStringList(QString("*.xml")),
                                 QDir::Files | QDir::NoSymLinks);

    LOG( "MapList::MapList() #xml: " << files.size(); )
    for (int i = 0; i < files.size(); ++i)
    {
        QString base = files[i].left(files[i].length()-4);
        LOG( "MapList::MapList(): Found " << files[i]; )
        map[base] = GpxIO::Instance()->ReadMapMetaFile(GetDrive() + QString(MAPDIR) + files[i]);
        map[base]->SetName(base);
    }

    files = directory.entryList(QStringList(QString("*.jpg")),
                                 QDir::Files | QDir::NoSymLinks);

    LOG( "MapList::MapList() #jpg: " << files.size(); )
    for (int i = 0; i < files.size(); ++i)
    {
        QString base = files[i].left(files[i].length()-4);
        if (!map.keys().contains(base))
        {
            LOG( "MapList::MapList(): Found " << files[i]; )
            map[base] = new MapMetaData();
            map[base]->SetName(base);
            GpxIO::Instance()->WriteMapMetaFile(*map[base]);
        }
    }
}

QStringList MapList::FindMapsForPosition(const WayPoint& w)
{
	LOG( "QMapList::FindMapsForPosition()"; )

	QStringList result;
	QStringList keys = MapNames();
	for (int i=0; i<keys.size(); ++i)
	{
		if (map[keys[i]]->IsPositionOnMap(w))
		{
			LOG( "QMapList::FindMapsForPosition(): " << keys[i]; )
			result.append(keys[i]);
		}
	}
	return result;
}

bool MapMetaData::WgsArea(double& lat1, double& lon1, double& lat2, double& lon2)
{
    if (!iscalibrated) return false;
    //LOG( "MapMetaData::WgsArea(): " << width << " " << height; )

    XY2Wgs(0,0,lat1,lon1);
    XY2Wgs(resolution.Width(),resolution.Height(),lat2,lon2);

    return true;
}

bool MapMetaData::XY2Wgs(double ax, double ay, double& alat, double& alon)
{
    if (!iscalibrated) return false;

    alon = (ax - x) * x2lon + lon;
    alat = (ay - y) * y2lat + lat;

    return true;
}

bool MapMetaData::Wgs2XY(double alat, double alon, double& ax, double& ay)
{
    if (!iscalibrated) return false;

    ax = (alon - lon) * lon2x + x;
    ay = (alat - lat) * lat2y + y;
    return true;
}

bool MapMetaData::IsPositionOnMap(double alat, double alon)
{
    double lat1,lat2,lon1,lon2;
    
    LOG( "MapMetaData::IsPositionOnMap(" << alat << "," << alon << ")"; )
    if (!iscalibrated)                   return false;
    if (resolution.Width() == 0)         return false;
    if (resolution.Height() == 0)        return false;
    if (!WgsArea(lat1,lon1,lat2,lon2))   return false;
    if (!IsValueInRange(alat,lat1,lat2)) return false;
    if (!IsValueInRange(alon,lon1,lon2)) return false;

    LOG( "MapMetaData::IsPositionOnMap(): Yes!"; )
    return true;
}

void MapMetaData::Calibrate()
{
	LOG( "MapMetaData::Calibrate()"; )
	iscalibrated = false;
	if (refpoints.length() < 2) return;
	
	int i=0;
	int j=0;
	bool found = false;
	while (!found && (i<refpoints.length()))
	{
		while (!found && (j < refpoints.length()))
		{
			found = IsValidRefpointPair(i,j);
			j++;
		}
		i++;
	}
	
	if (!found) return;
	
	CalculateIndexesFromRefpoints(--i,--j);
}

void MapMetaData::CalculateIndexesFromRefpoints(int i, int j)
{
    LOG2( "MapMetaData::CalculateIndexesFromRefpoints(" << i << "," << j << ")"; )
    LOG2( "MapMetaData::CalculateIndexesFromRefpoints(): " << refpoints[i].X() << " " << refpoints[i].Y() << " " << refpoints[i].Latitude() << " " << refpoints[i].Longitude(); )
    LOG2( "MapMetaData::CalculateIndexesFromRefpoints(): " << refpoints[j].X() << " " << refpoints[j].Y() << " " << refpoints[j].Latitude() << " " << refpoints[j].Longitude(); )
    int dx = refpoints[j].X() - refpoints[i].X();
    int dy = refpoints[j].Y() - refpoints[i].Y();
    double dlon = refpoints[j].Longitude() - refpoints[i].Longitude();
    double dlat = refpoints[j].Latitude() - refpoints[i].Latitude();
    LOG( "MapMetaData::CalculateIndexesFromRefpoints(): " << dx << " " << dy << " " << dlon << " " << dlat; )
    // 20 -40 -0.00181667 0.00203333
    
    double theta = (atan2(dy,dx) * 180 / PI) + 90;
    if (theta > 180)
        theta -= 360;

    double d,b;
    CalculateDistanceAndBearing(
        refpoints[i].Latitude(),refpoints[i].Longitude(),
        refpoints[j].Latitude(),refpoints[j].Longitude(),
        d,b);

    double dtheta = b - theta;
    if (dtheta > 180)
        dtheta -= 360;

    x = refpoints[i].X();
    y = refpoints[i].Y();
    lat = refpoints[i].Latitude();
    lon = refpoints[i].Longitude();
    x2lon = dlon/dx;
    y2lat = dlat/dy;
    lon2x = dx/dlon;
    lat2y = dy/dlat;
    iscalibrated = true;
    LOG2( "MapMetaData::CalculateIndexesFromRefpoints(): " << x << " " << y << " " << lat << " " << lon <<; )
    // 796 1012 61.4516 23.8541
    
    LOG2( "MapMetaData::CalculateIndexesFromRefpoints(): " << x2lon << " " << y2lat << " " << lat2y << " " << lon2x; )
    // -9.08333e-5 -5.08333e-5 -19672.1 -11009.2
}

bool MapMetaData::IsValidRefpointPair(int i, int j)
{
    if (i == j) return false;
    LOG( "MapMetaData::IsValidRefpointPair(" << i << "," << j << ")"; )

    if ((refpoints[i].X() != refpoints[j].X()) &&
        (refpoints[i].Y() != refpoints[j].Y()) &&
        (refpoints[i].Latitude() != refpoints[j].Latitude()) &&
        (refpoints[i].Longitude() != refpoints[j].Longitude()))
        return true;

    LOG( "MapMetaData::IsValidRefpointPair(): No!"; )
    return false;
}
