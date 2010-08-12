/*
 * datamonitor.cpp
 *
 *  Created on: 30 jul 2010
 *      Author: Mark Hurenkamp
 */

#include <QTime>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfo>
#include <QGeoSatelliteInfoSource>
#include <QCompass>
#include <QCompassReading>
#include "datamonitor.h"
#include "waypointlist.h"
#include "routelist.h"
#include "tracklist.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

QMonitorOptions::QMonitorOptions(QDialog* d)
: QWidget(d)
, settings("karpeer.net","qTracker",this)
{
    LOG( "QMonitorOptions::QMonitorOptions()"; )
    		
	montype       = settings.value("monitor/type",0).toInt();
	wptname       = settings.value("monitor/waypoint","").toString();
	rtename       = settings.value("monitor/route","").toString();
	trkname       = settings.value("monitor/track","").toString();
	
	QVBoxLayout *main = new QVBoxLayout();

	typegroup = new QGroupBox("Monitor");
	typebuttons = new QButtonGroup(typegroup);
	QRadioButton* none =     new QRadioButton(tr("None"));
	QRadioButton* waypoint = new QRadioButton(tr("Waypoint"));
	QRadioButton* route =    new QRadioButton(tr("Route"));
	QRadioButton* track =    new QRadioButton(tr("Track"));
	typebuttons->addButton(none,0);
	typebuttons->addButton(waypoint, 1);
	typebuttons->addButton(route,    2);
	typebuttons->addButton(track,    3);
	QVBoxLayout *typebox = new QVBoxLayout();
	typebox->addWidget(none);
	typebox->addWidget(waypoint);
	typebox->addWidget(route);
	typebox->addWidget(track);
	typegroup->setLayout(typebox);
	typegroup->show();

	filler = new QWidget;
	filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	QStringList wptlist = WayPointList::Instance().Keys();
	wptcombo = new QListWidget();
	wptcombo->addItems(wptlist);
    if ((wptlist.length()>0) && (wptlist.contains(wptname)))
        wptcombo->setCurrentRow(wptlist.indexOf(wptname));
    
	QStringList rtelist = RouteList::Instance()->Keys();
	rtecombo = new QListWidget();
	rtecombo->addItems(rtelist);
    if ((rtelist.length()>0) && (rtelist.contains(wptname)))
        rtecombo->setCurrentRow(rtelist.indexOf(wptname));
	
	QStringList trklist = TrackList::Instance()->Keys();
	trkcombo = new QListWidget();
	trkcombo->addItems(trklist);
    if ((trklist.length()>0) && (trklist.contains(trkname)))
        trkcombo->setCurrentRow(trklist.indexOf(trkname));
	
	switch (montype)
	{
		default:
		case 0:
			none->setChecked(true);
			noneselected();
			break;
		case 1: 
			waypoint->setChecked(true);
			waypointselected();
			break;
		case 2: 
			route->setChecked(true);
			routeselected();
			break;
		case 3: 
			track->setChecked(true);
			trackselected();
			break;
	}

	QPushButton* apply =  new QPushButton(tr("Apply"));
	QPushButton* cancel =  new QPushButton(tr("Cancel"));
	QHBoxLayout* buttonbox =  new QHBoxLayout();
	buttonbox->addWidget(apply);
	buttonbox->addWidget(cancel);
	
	QVBoxLayout* topleft = new QVBoxLayout();
	QVBoxLayout* bottomright = new QVBoxLayout();
    center = new QBoxLayout(QBoxLayout::LeftToRight);
	
	topleft->addWidget(typegroup);
	topleft->addWidget(filler);
	bottomright->addWidget(wptcombo);
	bottomright->addWidget(rtecombo);
	bottomright->addWidget(trkcombo);
	//main->addWidget(filler);
	center->addLayout(topleft);
	center->addLayout(bottomright);
	main->addLayout(center);
	main->addLayout(buttonbox);

    connect(none,    SIGNAL(clicked()),this,SLOT(noneselected()));
    connect(waypoint,SIGNAL(clicked()),this,SLOT(waypointselected()));
    connect(route,   SIGNAL(clicked()),this,SLOT(routeselected()));
    connect(track,   SIGNAL(clicked()),this,SLOT(trackselected()));

	connect(apply,   SIGNAL(clicked()),this,SLOT(apply()));
	connect(cancel,  SIGNAL(clicked()),d,SLOT(reject()));
	setLayout(main);
}

QMonitorOptions::~QMonitorOptions()
{
    LOG( "QMonitorOptions::~QMonitorOptions()"; )
}

void QMonitorOptions::resizeEvent( QResizeEvent * event )
{
    LOG( "QMonitorOptions::resizeEvent()"; )
    if (!center) return;
    
    if (event->size().width() < event->size().height())
        center->setDirection(QBoxLayout::TopToBottom);
    else
        center->setDirection(QBoxLayout::LeftToRight);

    QWidget::resizeEvent(event);
    updateGeometry();
}


void QMonitorOptions::apply()
{
    LOG( "QMonitorOptions::apply()"; )
	montype = typebuttons->checkedId();
	switch (montype)
	{
		case 1:
			wptname = wptcombo->currentItem()->text();
			break;
		case 2:
			rtename = rtecombo->currentItem()->text();
			break;
		case 3:
			trkname = trkcombo->currentItem()->text();
			break;
		default:
			break;
	}
	settings.setValue("monitor/type",montype);
	settings.setValue("monitor/waypoint",wptname);
	settings.setValue("monitor/route",rtename);
	settings.setValue("monitor/track",trkname);
	settings.sync();
	emit changed();
}

void QMonitorOptions::noneselected()
{
    LOG( "QMonitorOptions::noneselected()"; )
	wptcombo->hide();
	rtecombo->hide();
	trkcombo->hide();
	update();
}

void QMonitorOptions::waypointselected()
{
    LOG( "QMonitorOptions::waypointselected()"; )
	wptcombo->show();
	rtecombo->hide();
	trkcombo->hide();
	update();
}

void QMonitorOptions::routeselected()
{
    LOG( "QMonitorOptions::routeselected()"; )
	wptcombo->hide();
	rtecombo->show();
	trkcombo->hide();
	update();
}

void QMonitorOptions::trackselected()
{
    LOG( "QMonitorOptions::trackselected()"; )
	wptcombo->hide();
	rtecombo->hide();
	trkcombo->show();
	update();
}



DataMonitor* DataMonitor::instance = 0;
DataMonitor& DataMonitor::Instance() 
{ 
	if (!instance) 
		instance = new DataMonitor(); 
	return *instance; 
}

DataMonitor::DataMonitor()
: strategy(0)
, settings("karpeer.net","qTracker",this)
, compass(0)
, reading(0)
{
    LOG( "DataMonitor::DataMonitor()"; )
	heading    = new HeadingFilter(1);
	speed      = new AverageFilter(5);
	altitude   = new AverageFilter(10);
	horizontal = new AverageFilter(5);
	vertical   = new AverageFilter(5);
	
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
    
    satsource = QGeoSatelliteInfoSource::createDefaultSource(this);
    if (satsource) {
        connect(satsource, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(UpdateSatsInView(QList<QGeoSatelliteInfo>)));
        connect(satsource, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
                this, SLOT(UpdateSatsInUse(QList<QGeoSatelliteInfo>)));
        satsource->startUpdates();
    }

#ifndef DISABLE_COMPASS    
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
#endif
    {
        LOG( "DataMonitor::DataMonitor(): No compass"; )
    }
    
	int montype       = settings.value("monitor/type",0).toInt();
	QString wptname   = settings.value("monitor/waypoint","").toString();
	if (montype==1)
		SetStrategy(new WayPointStrategy(wptname));
	
	connect(heading,    SIGNAL(ValueUpdated(double)), this, SIGNAL(HeadingUpdated(double)));
	connect(speed,      SIGNAL(ValueUpdated(double)), this, SIGNAL(SpeedUpdated(double)));
	connect(altitude,   SIGNAL(ValueUpdated(double)), this, SIGNAL(AltitudeUpdated(double)));
	connect(horizontal, SIGNAL(ValueUpdated(double)), this, SIGNAL(HorizontalUpdated(double)));
	connect(vertical,   SIGNAL(ValueUpdated(double)), this, SIGNAL(VerticalUpdated(double)));
}

DataMonitor::~DataMonitor()
{
    LOG( "DataMonitor::~DataMonitor()"; )
}

void DataMonitor::OnPositionUpdate(const QGeoPositionInfo& info)
{
    LOG( "DataMonitor::PositionUpdate()"; )
    		
	emit PositionUpdated(info);
	emit AltitudeUpdated(info.coordinate().altitude());
	
    if (info.hasAttribute(QGeoPositionInfo::GroundSpeed))
    	speed->NewValue(info.attribute(QGeoPositionInfo::GroundSpeed)*3.6);
        //emit SpeedUpdated(info.attribute(QGeoPositionInfo::GroundSpeed)*3.6);
    
    if ( IsUsingGPSCompass() ) 
	{
        if (info.hasAttribute(QGeoPositionInfo::Direction))
        {
        	double a = info.attribute(QGeoPositionInfo::Direction);
        	//emit HeadingUpdated(a);
        	heading->NewValue(a);
    	    if (strategy) strategy->OnHeadingUpdate(a);
        }
	}
    		
	if (strategy) strategy->OnPositionUpdate(info);
}

void DataMonitor::OnHeadingUpdate()
{
    LOG( "DataMonitor::OnHeadingUpdate()"; )
    		
	if ( IsUsingGPSCompass() ) return;
    
	int a = reading->azimuth();
	heading->NewValue(a);
    //emit HeadingUpdated(a);
	if (strategy) strategy->OnHeadingUpdate(a);
}

void DataMonitor::OnTimeUpdate()
{
    LOG( "DataMonitor::OnTimeUpdate()"; )
    		
	QTime time = QTime::currentTime();
    //emit TimeUpdated(time);
	if (strategy) strategy->OnTimeUpdate(time);
}

void DataMonitor::SetStrategy(MonitorStrategy *s)
{
    LOG( "DataMonitor::SetStrategy()"; )
    		
	if (strategy) delete strategy;
	strategy = s;
	connect(strategy,SIGNAL(BearingUpdated(double)),this,SIGNAL(BearingUpdated(double)));
	connect(strategy,SIGNAL(DistanceUpdated(double)),this,SIGNAL(DistanceUpdated(double)));
	connect(strategy,SIGNAL(TimeUpdated(long)),this,SIGNAL(TimeUpdated(long)));
}



WayPointStrategy::WayPointStrategy(const WayPoint& wpt)
: MonitorStrategy(1)
{
	name = wpt.Name();
	targetposition.setLatitude(wpt.Latitude());
	targetposition.setLongitude(wpt.Longitude());
}

WayPointStrategy::WayPointStrategy(const QString& wpt)
: MonitorStrategy(1)
{
	name = wpt;
	targetposition.setLatitude(WayPointList::Instance().GetItem(wpt).Latitude());
	targetposition.setLongitude(WayPointList::Instance().GetItem(wpt).Longitude());
}

void WayPointStrategy::OnTimeUpdate(const QTime& time)
{
    if (currentinfo.hasAttribute(QGeoPositionInfo::GroundSpeed))
    {
        double speed = DataMonitor::Instance().Speed();
    	double distance = currentposition.distanceTo(targetposition);
        LOG( "WayPointStrategy::OnPositionUpdate() Distance: " << distance << " Speed: " << speed << " Distance/Speed: " << distance/speed; )
        emit TimeUpdated(distance/speed);
    }
}

void WayPointStrategy::OnPositionUpdate(const QGeoPositionInfo& info)
{
	currentinfo = info;
	currentposition = info.coordinate();
	double distance = currentposition.distanceTo(targetposition);
	emit BearingUpdated(currentposition.azimuthTo(targetposition));
	emit DistanceUpdated(distance);
	OnTimeUpdate(QTime::currentTime());
}
