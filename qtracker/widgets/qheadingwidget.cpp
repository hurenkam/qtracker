#include <QWidget>
#include <QCompass>
#include "qheadingwidget.h"
#include "qgaugewidget.h"
#include "ui.h"
#include "qcompassdialog.h"
#include "datamonitor.h"


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
	, heading(0)
	, azimuth(0)
{
    LOG( "QHeadingWidget::QHeadingWidget()"; )
	ReadSettings();
    timer = new QTimer(this);
    
    connect(&DataMonitor::Instance(), SIGNAL(HeadingUpdated(double)), this, SLOT(UpdateHeading(double)));
    connect(&DataMonitor::Instance(), SIGNAL(BearingUpdated(double)), this, SLOT(UpdateAzimuth(double)));
    connect(timer, SIGNAL(timeout()), this, SLOT(timerStep()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(SelectOptions()));
}

void QHeadingWidget::ReadSettings()
{
	settings.sync();
	view    = settings.value("compass/view",0).toInt();
    LOG( "QHeadingWidget::ReadSettings()  View: " << view; )
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

void QHeadingWidget::paintEvent(QPaintEvent *event)
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
    stepsdial = STEPS-1;
    deltadial = delta / STEPS;
    curdial += deltadial;

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
    stepsneedle = STEPS-1;
    deltaneedle = delta / STEPS;
    curneedle += deltaneedle;

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
    stepsring = STEPS-1;
    deltaring = delta / STEPS;
    curring += deltaring;

    timer->start(TIMER);
}
