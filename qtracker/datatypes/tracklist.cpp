/*
 * geodata.cpp
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#include "gpxio.h"
#include "ui.h"
#include "waypointlist.h"
#include "tracklist.h"
#include "datamonitor.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

TrackList* TrackList::instance = 0;

TrackList::TrackList()
: settings("karpeer.net","qTracker",this)
, recordtrack(0)
, isrecording(false)
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
	FindFiles();
    prevtime = QDateTime::fromTime_t(0).toUTC();
    prevpos = QGeoCoordinate(-200,-200);

	connect(&DataMonitor::Instance(), SIGNAL(PositionUpdated(QGeoPositionInfo)), this, SLOT(UpdatePosition(QGeoPositionInfo)));
};

TrackList::~TrackList()
{
}

void TrackList::SaveSettings()
{
	QStringList names = VisibleKeys();
	settings.beginWriteArray("trk/list",names.length());
	for (int i=0; i<names.length(); i++)
	{
		settings.setArrayIndex(i);
		settings.setValue("name",names[i]);
	}
	settings.endArray();
	settings.sync();
} 

void TrackList::AddTrack(Track* t)
{ 
	if (!t) return;
	
	map[t->Name()]=t; 
	if (!trackfiles.contains(t->Name()))
    {
        GpxIO::Instance()->WriteTrackFile(*t);
        FindFiles();
    }
	emit added(t->Name()); 
}

void TrackList::AddMetaData(AreaMetaData*)
{ 
}

QStringList TrackList::Keys()
{ 
	return trackfiles; 
}

QStringList TrackList::FindFiles()
{
	QDir directory = QDir(GetDrive() + QString(TRACKDIR));
	QStringList files = directory.entryList(QStringList(QString("*.gpx")),
															 QDir::Files | QDir::NoSymLinks);

	LOG( "QTrackListTab::TrackFiles() #gpx: " << files.size() << "\n"; )
	for (int i = 0; i < files.length(); ++i)
	{
			files[i] = files[i].left(files[i].length()-4);
	}
	trackfiles = files;
	return files;
}

QStringList TrackList::VisibleKeys()
{ 
	return map.keys(); 
}

QStringList TrackList::HiddenKeys()              
{ 
	QStringList l = Keys();
	QStringList v = VisibleKeys();
	for (int i=0; i<l.length(); i++) 
		if (v.contains(l[i])) 
			l.removeAll(l[i]); 
	return l; 
}

Track& TrackList::GetItem(const QString& k) const        
{ 
	if (!map.keys().contains(k)) qFatal("key not found");
	
	return *map[k]; 
}

void TrackList::Hide(const QString& name)         
{ 
    LOG( "TrackList::HideTrack()"; )
    if ((recordtrack) && (recordtrack->Name()==name)) return;
    
	Track* t = map[name]; 
	map.remove(name); 
	delete t; 
    emit invisible(name); 
}

void TrackList::Show(const QString& name)         
{
    QString filename = TRACKDIR + name + ".gpx";
    GpxIO::Instance()->ImportGpxFile(filename);
    emit visible(name); 
}

void TrackList::Delete(const QString& name)         
{
	if (map.keys().contains(name))
		Hide(name);
	
	if (trackfiles.contains(name))
		trackfiles.removeAll(name);
	
	QString filename = name + ".gpx";
    QDir dir = QDir(TRACKDIR);
	dir.remove(filename);
	emit deleted(name);
}

void TrackList::Start(const QString& key, int d, int t)
{
	isrecording = true;
	recordtrack = new Track();
	recordtrack->SetName(key);
	GpxIO::Instance()->WriteTrackFile(*recordtrack);
	AddTrack(recordtrack);
	FindFiles();
	UpdateInterval(d,t);
    emit visible(key); 
}

void TrackList::UpdateInterval(int d, int t)
{
	timeinterval = t;
	distinterval = d;
}

void TrackList::Stop()
{
	GpxIO::Instance()->WriteTrackFile(*recordtrack);
	recordtrack = 0;
	isrecording = false;
}

void TrackList::UpdatePosition(const QGeoPositionInfo& info)
{
    LOG( "TrackList::UpdatePosition()"; )
	if (!isrecording) return;
	
	QDateTime curtime = QDateTime::currentDateTime().toUTC();
	QString timestamp = curtime.toString("yyyy-MM-ddThh:mm:ssZ");
	QGeoCoordinate pos = info.coordinate();

	int deltatime = 0;
	double deltadistance = 0;
	
	deltatime = prevtime.secsTo(curtime);
	if (prevpos.isValid())
	{
		deltadistance = prevpos.distanceTo(pos);
	}
	
	if ((timeinterval==0) && (distinterval==0))
	{
		recordtrack->AddPoint(new WayPoint(pos.latitude(),pos.longitude(),pos.altitude(),timestamp));
		prevtime = curtime;
		prevpos = pos;
	}
	else if ((timeinterval!=0) && (timeinterval < deltatime))
	{
		recordtrack->AddPoint(new WayPoint(pos.latitude(),pos.longitude(),pos.altitude(),timestamp));
		prevtime = curtime;
		prevpos = pos;
	}
	else if ((distinterval!=0) && ((!prevpos.isValid()) || (distinterval < deltadistance)))
	{
		recordtrack->AddPoint(new WayPoint(pos.latitude(),pos.longitude(),pos.altitude(),timestamp));
		prevtime = curtime;
		prevpos = pos;
	}
}
