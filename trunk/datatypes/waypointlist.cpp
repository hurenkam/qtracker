/*
 * geodata.cpp
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#include "gpxio.h"
#include "ui.h"
#include "waypointlist.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

WayPointList* WayPointList::instance = 0;

using namespace geodata;

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
