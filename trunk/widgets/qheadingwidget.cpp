#include <QWidget>
#include <QCompass>
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
	, compass(0)
	, reading(0)
	, heading(0)
	, azimuth(0)
{
    LOG( "QHeadingWidget::QHeadingWidget()"; )
	ReadSettings();
    timer = new QTimer(this);
    
    possource = QGeoPositionInfoSource::createDefaultSource(this);
    if (possource) {
        possource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        possource->setUpdateInterval(500);
        connect(possource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(UpdatePosition(QGeoPositionInfo)));
        possource->startUpdates();
    }
    else
    {
        LOG( "QHeadingWidget::QHeadingWidget(): No possource"; )
    }
    
    compass = new QCompass();
    if (compass)
    {
    	compass->setDataRate(1);
    	if (!compass->start())
    	{
            LOG( "QHeadingWidget::QHeadingWidget(): Not started"; )
    	}
    	else
    	{
			reading = compass->reading();
			if (reading)
			{
				UpdateHeading();
				connect(compass, SIGNAL(readingChanged()), this, SLOT(UpdateHeading()));
			}
			else
			{
				LOG( "QHeadingWidget::QHeadingWidget(): No reading"; )
			}
    	}
    }
    else
    {
        LOG( "QHeadingWidget::QHeadingWidget(): No compass"; )
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
    LOG( "QHeadingWidget::ReadSettings()  Source: " << source << "  View: " << view << "  Type: " << montype << "  Wpt: " << wptname; )
	
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
    LOG( "QHeadingWidget::SelectOptions()"; )
	QCompassDialog *dialog;
	dialog = new QCompassDialog(this);
	dialog->setModal(true);
	connect(dialog,SIGNAL(accepted()),this,SLOT(ReadSettings()));
	dialog->show();
}

void QHeadingWidget::UpdatePosition(const QGeoPositionInfo& info)
{
    LOG( "QHeadingWidget::UpdatePosition(" << info << ")"; )
	
    if ((source==1) && (info.hasAttribute(QGeoPositionInfo::Direction)))
    	UpdateHeading(info.attribute(QGeoPositionInfo::Direction));
    
	if (montype==1)
	{
		WayPoint wpt(info.coordinate().latitude(),info.coordinate().longitude());
		UpdateAzimuth(wpt.bearing(&monitor));
	}
}

void QHeadingWidget::UpdateHeading()
{
	if (source!=0) return;
	if (!reading) return;
	
    LOG( "QHeadingWidget::UpdateHeading()"; )
    UpdateHeading(reading->azimuth());
}

void QHeadingWidget::UpdateHeading(double h)
{
    LOG( "QHeadingWidget::UpdateHeading(" << h << ")"; )
    if (h==heading) return;
    
	heading = h;
	UpdateDials();
}

void QHeadingWidget::UpdateAzimuth(double a)
{
    LOG( "QHeadingWidget::UpdateAzimuth(" << a << ")"; )
    if (a==azimuth) return;
    
	azimuth = a;
	UpdateDials();
}

void QHeadingWidget::UpdateDials()
{
    LOG( "QHeadingWidget::UpdateDials()"; )
    		
	if (view==1) // Heading up
	{
	    LOG( "QHeadingWidget::UpdateDials(): Heading up"; )
	    SetDial(360.0-heading);
	    SetNeedle(0);
        SetRing(azimuth-heading);
	}
	else // North up
	{
	    LOG( "QHeadingWidget::UpdateDials(): North up"; )
	    SetDial(0);
	    SetNeedle(heading);
        SetRing(azimuth);
	}
}

void QHeadingWidget::timerStep()
{
    if ((stepsdial == 0) && (stepsneedle==0) && (stepsring==0))
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
    LOG( "QHeadingWidget::SetDial(" << v << ")"; )
    timer->stop();

    double delta = setdial - curdial;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    setdial = v;
    stepsdial = STEPS;
    deltadial = delta / stepsdial;

    timer->start(TIMER);
}

void QHeadingWidget::SetNeedle(double v)
{
    LOG( "QHeadingWidget::SetNeedle(" << v << ")"; )
    timer->stop();

    double delta = setneedle - curneedle;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    setneedle = v;
    stepsneedle = STEPS;
    deltaneedle = delta / stepsneedle;

    timer->start(TIMER);
}

void QHeadingWidget::SetRing(double v)
{
    LOG( "QHeadingWidget::SetRing(" << v << ")"; )
    timer->stop();

    double delta = setring - curring;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    setring = v;
    stepsring = STEPS;
    deltaring = delta / stepsring;

    timer->start(TIMER);
}
