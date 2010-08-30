/*
 * geodata.cpp
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#include "gpxio.h"
#include "ui.h"
#include "waypointlist.h"
//#include "GeoCoords.hpp"
//#include "TransverseMercator.hpp"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

WayPointList* WayPointList::instance = 0;

//using namespace GeographicLib;
/*
static const double rd_a    = 6377397.155;
static const double rd_b    = 299.1528128;
static const double rd_r    = rd_a/(rd_a-rd_b);
static const double rd_k0   = 0.9999079;
static const double rd_lat0 = 52.15616055555555;
static const double rd_lon0 = 5.38763888888889;
static const double rd_fe   = 155000;
static const double rd_fn   = 463000;

class RijksDriehoek
{
public:
	RijksDriehoek()
	: OSGB(rd_a, rd_r, rd_k0)
	{
		double gamma, k;
		OSGB.Forward(rd_lon0, rd_lat0, rd_lon0, x0, y0, gamma, k);
		x0 -= rd_fe;
		y0 -= rd_fn;
	}
	void Forward(double lat, double lon, double& x, double& y)
    {
		double gamma, k;
		OSGB.Forward(rd_lon0,lat,lon,x,y,gamma,k);
        x -= x0;
        y -= y0;
    }
	void Reverse(double x, double y, double& lat, double& lon)
    {
		double gamma, k;
        x += x0;
        y += y0;
        OSGB.Reverse(rd_lon0, x, y, lat, lon, gamma, k);
    }
    void Forward(const WayPoint& w, QString& pos)
    {
    	double x,y;
    	Forward(w.Latitude(),w.Longitude(),x,y);
    	pos = "RD " + QString::number(x) + " " + QString::number(y);
    }
    bool Reverse(const QString& pos, WayPoint& result)
    {
    	QStringList list = pos.split(" ", QString::SkipEmptyParts);
    	if ((list.length()!=3) || (list[0]!="RD"))
    		return false;
    	
		result.SetLatitude(list[1].toDouble());
		result.SetLongitude(list[2].toDouble());
		return true;
    }
	
private:
	TransverseMercator OSGB;
	double x0, y0;
};
static RijksDriehoek RD;
*/

WayPoint::WayPoint(QString s, QString n)
: elevation(0), time(""), name(n)
{
/*
	//if (!RD.Reverse(s,*this))
	{
		GeographicLib::GeoCoords p(s.toStdString());
		latitude = p.Latitude();
		longitude = p.Longitude();
	}
*/
}

double WayPoint::distance(WayPoint* to)
{
	double d, b;
	geodata::CalculateDistanceAndBearing(Latitude(),Longitude(),to->Latitude(),to->Longitude(),d,b);
	return d;
}

QString WayPoint::distancestr(WayPoint* to)
{
	QString postfix("m");
	QString result;
    double d = distance(to);
    if (d>999)
    {
        d = d/1000;
        postfix = "km";
    }
    result.sprintf("%.1f",d);
    return result % postfix;
}

double WayPoint::bearing(WayPoint* to)
{
	double d, b;
	geodata::CalculateDistanceAndBearing(Latitude(),Longitude(),to->Latitude(),to->Longitude(),d,b);
	return b;
}

QString WayPoint::Representation(geodata::Datum datum) const
{
/*
    GeographicLib::GeoCoords p(Latitude(),Longitude());
    //std::string pos;
    QString pos;
    switch (datum)
    {
    	default:
    	case geodata::Wgs84_Geo: pos = QString::fromStdString(p.GeoRepresentation());    break;
    	case geodata::Wgs84_DMS: pos = QString::fromStdString(p.DMSRepresentation());    break;
    	case geodata::UTMUPS:    pos = QString::fromStdString(p.UTMUPSRepresentation()); break;
    	case geodata::MGRS:      pos = QString::fromStdString(p.MGRSRepresentation());   break;
    	//case geodata::RD:        RD.Forward(*this, pos);                                 break;
    }
	return pos;
*/
}

WayPointList& WayPointList::Instance() 
{ 
	if (!instance) 
		instance = new WayPointList(); 
	return *instance; 
}

WayPointList::WayPointList()
: settings("karpeer.net","qTracker",this) 
{ 
	instance = this;
	int length = settings.beginReadArray("wpt/list");
	visiblekeys.clear();
	for (int i=0; i<length; i++)
	{
		settings.setArrayIndex(i);
		WayPoint* w = new WayPoint();
		w->SetName      (settings.value("name").toString());
		w->SetLatitude  (settings.value("latitude").toDouble());
		w->SetLongitude (settings.value("longitude").toDouble());
		w->SetElevation (settings.value("elevation").toDouble());
		w->SetTime      (settings.value("time").toString());
		bool visible = settings.value("visible",false).toBool();
		AddWayPoint(w,visible);
	}
	settings.endArray();
};

WayPointList::~WayPointList() 
{ 
}

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

void WayPointList::AddWayPoint(WayPoint* w,bool visible)         
{ 
	map[w->Name()]=w;
	emit added(w->Name()); 
	if (visible)
		Show(w->Name());
}

void WayPointList::AddWayPoint(const WayPoint& w)   
{ 
	if (map.keys().contains(w.Name()))
        UpdateWayPoint(w.Name(),w);
    else
	    AddWayPoint(new WayPoint(w)); 
}

void WayPointList::UpdateWayPoint(const QString& orgname, const WayPoint& w)   
{
	map.remove(orgname);
	map[w.Name()]=new WayPoint(w);		
	if (visiblekeys.contains(orgname)) 
	{
		visiblekeys.removeAll(orgname);
		visiblekeys.append(w.Name());
	}
	emit updated(orgname,w.Name());
}

void WayPointList::RemoveWayPoint(const QString& s) 
{ 
	map.remove(s); 
	visiblekeys.removeAll(s); 
	emit removed(s); 
}

QStringList WayPointList::Keys()                    
{ 
	return map.keys(); 
}

void WayPointList::Hide(const QString& key)         
{ 
	if (visiblekeys.contains(key)) 
	{ 
		visiblekeys.removeAll(key); 
		emit invisible(key); 
	} 
}

void WayPointList::Show(const QString& key)         
{ 
	if (map.keys().contains(key)) 
	{ 
		visiblekeys.append(key); 
		emit visible(key); 
	} 
}

QStringList WayPointList::VisibleKeys()             
{ 
	return visiblekeys; 
}

QStringList WayPointList::HiddenKeys()              
{ 
	QStringList l = map.keys();  
	for (int i=0; i<l.length(); i++) 
		if (visiblekeys.contains(l[i])) 
			l.removeAll(l[i]); 
	return l; 
}

QStringList WayPointList::AreaKeys(Bounds a)        
{ 
	QStringList l = map.keys();  
	for (int i=0; i<l.length(); i++) 
		if (a.Contains(*map[l[i]]))     
			l.removeAll(l[i]); 
	return l; 
}

QStringList WayPointList::VisibleAreaKeys(Bounds a) 
{ 
	QStringList l = AreaKeys(a); 
	for (int i=0; i<l.length(); i++) 
		if (visiblekeys.contains(l[i])) 
			l.removeAll(l[i]); 
	return l; 
}

bool WayPointList::IsVisible(const QString& k)      
{ 
	return visiblekeys.contains(k); 
}

const WayPoint& WayPointList::GetItem(const QString& n)   
{ 
	return *map[n]; 
}

QString WayPointList::FileName()                    
{ 
	return QString(GetDrive() + QString(WAYPOINTDIR) + "waypoints.gpx"); 
}
