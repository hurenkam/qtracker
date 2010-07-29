#include <QWidget>
#include "qheadingwidget.h"
#include "qgaugewidget.h"
#include "ui.h"
#include "qcompassdialog.h"
#include "waypointlist.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

QHeadingWidget::QHeadingWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , settings("karpeer.net","qTracker",this)
    , setdial(0.0)
    , curdial(0.0)
    , deltadial(0.0)
    , stepsdial(0)
    , setneedle(0.0)
    , curneedle(0.0)
    , deltaneedle(0.0)
    , stepsneedle(0)
	, setring(0.0)
	, curring(0.0)
	, deltaring(0.0)
	, stepsring(0)
{
	ReadSettings();
    timer = new QTimer(this);
    
    possource = QGeoPositionInfoSource::createDefaultSource(this);
    if (possource) {
        possource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        possource->setUpdateInterval(500);
        connect(possource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(UpdatePosition(QGeoPositionInfo)));
        possource->startUpdates();
    }
    
    connect(timer, SIGNAL(timeout()), this, SLOT(timerStep()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(SelectOptions()));
}

void QHeadingWidget::ReadSettings()
{
	settings.sync();
	source  = settings.value("compass/source",0).toInt();
	view    = settings.value("compass/view",0).toInt();
	montype = settings.value("monitor/type",0).toInt();
	wptname = settings.value("monitor/waypoint","").toString();
	rtename = settings.value("monitor/route","").toString();
	trkname = settings.value("monitor/track","").toString();
	
	switch (montype)
	{
		default:
			break;
		case 1: // Waypoint
			QStringList keys = WayPointList::Instance().Keys();
			if (keys.contains(wptname))
			    monitor = WayPointList::Instance().GetItem(wptname);
			break;
	}
}

void QHeadingWidget::SelectOptions()
{
	QCompassDialog *dialog;
	dialog = new QCompassDialog(this);
	dialog->setModal(true);
	dialog->show();
	ReadSettings();
}

void QHeadingWidget::UpdatePosition(const QGeoPositionInfo& info)
{
    LOG( "QHeadingWidget::UpdatePosition()"; )
	
    if (info.hasAttribute(QGeoPositionInfo::Direction))
    {
    	double heading = info.attribute(QGeoPositionInfo::Direction);
    	double azimuth = 0;
    	if (montype==1)
    	{
    		WayPoint wpt(info.coordinate().latitude(),info.coordinate().longitude());
    		azimuth = wpt.bearing(&monitor);
    	}
    	
    	if (view==1) // Heading up
		{
		    SetDial(360.0-heading);
    	    SetNeedle(0);
    	    if (montype==1)
		        SetRing(azimuth-heading);
		}
    	else // North up
    	{
		    SetDial(0);
    	    SetNeedle(heading);
    	    if (montype==1)
		        SetRing(azimuth);
    	}
    }
}

void QHeadingWidget::timerStep()
{
    if ((stepsdial == 0) && (stepsneedle==0))
    {
        timer->stop();
        return;
    }

	if (stepsdial != 0)
	{
		curdial += deltadial;
		stepsdial -= 1;
	}

    if (stepsneedle != 0)
    {
        curneedle += deltaneedle;
        stepsneedle -= 1;
    }

    if (stepsring != 0)
    {
        curring += deltaring;
        stepsring -= 1;
    }
    update();
}

void QHeadingWidget::paintEvent(QPaintEvent *)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QPainter painter(this);
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);
    painter.translate(x,y);
    painter.rotate(curring);
    painter.drawImage(target, svgCompassRing, source);
    painter.rotate(-1 * curring);
    painter.rotate(curdial);

    painter.drawImage(target, svgHeading, source);
    painter.rotate(-1 * curdial);
    painter.rotate(curneedle);
    painter.drawImage(target, svgCompassNeedle, source);
}

void QHeadingWidget::SetDial(double v)
{
    timer->stop();

    double delta = setdial - curdial;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    setdial = v;
    stepsdial = 6;
    deltadial = delta / stepsdial;

    timer->start(330);
}

void QHeadingWidget::SetNeedle(double v)
{
    timer->stop();

    double delta = setneedle - curneedle;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    setneedle = v;
    stepsneedle = 6;
    deltaneedle = delta / stepsneedle;

    timer->start(330);
}

void QHeadingWidget::SetRing(double v)
{
    timer->stop();

    double delta = setring - curring;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    setring = v;
    stepsring = 6;
    deltaring = delta / stepsring;

    timer->start(330);
}
