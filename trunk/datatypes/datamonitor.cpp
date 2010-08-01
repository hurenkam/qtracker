/*
 * datamonitor.cpp
 *
 *  Created on: 30 jul 2010
 *      Author: Mark Hurenkamp
 */

#include <QDateTime>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QCompass>
#include <QCompassReading>
#include "datamonitor.h"
#include "waypointlist.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

DataMonitor* DataMonitor::instance = 0;
DataMonitor& DataMonitor::Instance() 
{ 
	if (!instance) 
		instance = new DataMonitor(); 
	return *instance; 
}

DataMonitor::DataMonitor()
: strategy(0)
{
    possource = QGeoPositionInfoSource::createDefaultSource(this);
    if (possource) {
        possource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        possource->setUpdateInterval(500);
        connect(possource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(OnPositionUpdate(QGeoPositionInfo)));
        possource->startUpdates();
    }
    else
    {
        LOG( "DataMonitor::DataMonitor(): No possource"; )
    }
    
    compass = new QCompass();
    if (compass)
    {
    	compass->setDataRate(1);
    	if (!compass->start())
    	{
            LOG( "DataMonitor::DataMonitor(): Not started"; )
    	}
    	else
    	{
			reading = compass->reading();
			if (reading)
			{
				OnHeadingUpdate();
				connect(compass, SIGNAL(readingChanged()), this, SLOT(OnHeadingUpdate()));
			}
			else
			{
				LOG( "DataMonitor::DataMonitor(): No reading"; )
			}
    	}
    }
    else
    {
        LOG( "DataMonitor::DataMonitor(): No compass"; )
    }
}

DataMonitor::~DataMonitor()
{
}

void DataMonitor::OnPositionUpdate(const QGeoPositionInfo& info)
{
	emit PositionUpdated(info);
	if (strategy) strategy->OnPositionUpdate(info);
}

void DataMonitor::OnHeadingUpdate()
{
	int a = reading->azimuth();
    emit HeadingUpdated(a);
	if (strategy) strategy->OnHeadingUpdate(a);
}

void DataMonitor::OnTimeUpdate()
{
	QDateTime time = QDateTime::currentDateTime().toUTC();
    //emit TimeUpdated(time);
	if (strategy) strategy->OnTimeUpdate(time);
}

void DataMonitor::SetStrategy(MonitorStrategy *s)
{
	if (strategy) delete strategy;
	strategy = s;
	connect(strategy,SIGNAL(BearingUpdated(double)),this,SIGNAL(BearingUpdated(double)));
	connect(strategy,SIGNAL(DistanceUpdated(double)),this,SIGNAL(DistanceUpdated(double)));
	connect(strategy,SIGNAL(TimeUpdated(const QDateTime&)),this,SIGNAL(TimeUpdated(const QDateTime&)));
}



WayPointStrategy::WayPointStrategy(const WayPoint& wpt)
{
	name = wpt.Name();
	targetposition.setLatitude(wpt.Latitude());
	targetposition.setLongitude(wpt.Longitude());
}

WayPointStrategy::WayPointStrategy(const QString& wpt)
{
	name = wpt;
	targetposition.setLatitude(WayPointList::Instance().GetItem(wpt).Latitude());
	targetposition.setLongitude(WayPointList::Instance().GetItem(wpt).Longitude());
}

void WayPointStrategy::OnPositionUpdate(const QGeoPositionInfo& info)
{
	currentposition = info.coordinate();
	emit BearingUpdated(currentposition.azimuthTo(targetposition));
	emit DistanceUpdated(currentposition.distanceTo(targetposition));
}
