/*
 * geodata.cpp
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#include "gpxio.h"
#include "ui.h"
#include "routelist.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

RouteList* RouteList::instance = 0;

RouteList::RouteList()
: settings("karpeer.net","qTracker",this)
{
	instance = this;
	int length = settings.beginReadArray("rte/list");
	for (int i=0; i<length; i++)
	{
		settings.setArrayIndex(i);
		QString filename = GetDrive() + ROUTEDIR + settings.value("name").toString() + ".gpx";
		LOG( "RouteList::RouteList():" << filename; )
		GpxIO::Instance()->ImportGpxFile(filename);
	}
	settings.endArray();
	FindFiles();
}

RouteList::~RouteList()
{
}

QStringList RouteList::FindFiles()
{
	QDir directory = QDir(GetDrive() + QString(ROUTEDIR));
	QStringList files = directory.entryList(QStringList(QString("*.gpx")),
															 QDir::Files | QDir::NoSymLinks);

	LOG( "RouteList::FindFiles() #gpx: " << files.size() << "\n"; )
	for (int i = 0; i < files.length(); ++i)
	{
			files[i] = files[i].left(files[i].length()-4);
	}
	routefiles = files;
	return files;
}

void RouteList::SaveSettings()
{
	QStringList names = VisibleKeys();
	settings.beginWriteArray("rte/list",names.length());
	for (int i=0; i<names.length(); i++)
	{
		settings.setArrayIndex(i);
		settings.setValue("name",names[i]);
	}
	settings.endArray();
	settings.sync();
}

void RouteList::Hide(const QString& key)
{
    LOG( "RouteList::Hide()"; )
    
	Route* r = map[key]; 
	map.remove(key); 
	delete r; 
    emit invisible(key); 
}

void RouteList::Show(const QString& key)
{
    QString filename = ROUTEDIR + key + ".gpx";
    GpxIO::Instance()->ImportGpxFile(filename);
    emit visible(key); 
}

void RouteList::Delete(const QString& key)
{
	if (map.keys().contains(key))
		Hide(key);
	
	if (routefiles.contains(key))
		routefiles.removeAll(key);
	
	QString filename = key + ".gpx";
    QDir dir = QDir(ROUTEDIR);
	dir.remove(filename);
	emit deleted(key);
}

void RouteList::AddRoute(Route* r)
{
	if (!r) return;
	
	map[r->Name()]=r;
	if (!routefiles.contains(r->Name()))
    {
        GpxIO::Instance()->WriteRouteFile(*r);
        FindFiles();
    }
	emit added(r->Name()); 
}

void RouteList::AddMetaData(AreaMetaData* m)
{
}

QStringList RouteList::Keys()
{
	return routefiles; 
}

QStringList RouteList::VisibleKeys()
{
	return map.keys(); 
}

QStringList RouteList::HiddenKeys()
{
	QStringList l = Keys();
	QStringList v = VisibleKeys();
	for (int i=0; i<l.length(); i++) 
		if (v.contains(l[i])) 
			l.removeAll(l[i]); 
	return l; 
}

QStringList RouteList::AreaKeys(Bounds a)
{
}

QStringList RouteList::VisibleAreaKeys(Bounds a)
{
}

bool RouteList::IsVisible(const QString& k)
{
}

Route& RouteList::GetItem(const QString& k) const
{
	if (!map.keys().contains(k)) qFatal("key not found");
	
	return *map[k]; 
}
