#include <QtGui>
#include <QGeoPositionInfo>
#include "datamonitor.h"
#include "qgaugewidget.h"
#include "qspeedwidget.h"
#include "ui.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

QSpeedWidget::QSpeedWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , scale(10)
    , steps(0)
    , speed(0)
    , delta(0.0)
    , curspeed(0.0)
    , setspeed(0.0)
    , distance(0.0)
    , prevpos(0)
    , mindist(25)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerStep()));
	connect(&DataMonitor::Instance(), SIGNAL(PositionUpdated(QGeoPositionInfo)), this, SLOT(UpdatePosition(QGeoPositionInfo)));
    connect(this, SIGNAL(longTap()), this, SLOT(Reset()));
}

void QSpeedWidget::Reset()
{
    SetDistance(0);
}

void QSpeedWidget::UpdatePosition(const QGeoPositionInfo& info)
{
    LOG( "QAltitudeWidget::UpdatePosition()"; )
    QGeoCoordinate curpos = info.coordinate();
    
    if (info.hasAttribute(QGeoPositionInfo::GroundSpeed))
        SetSpeed(info.attribute(QGeoPositionInfo::GroundSpeed)*3.6);
    
	if (prevpos)
	{
		double delta = prevpos->distanceTo(curpos);
		if (delta > mindist)
		{
			distance += delta/1000.0;
			prevpos->setLatitude(curpos.latitude());
			prevpos->setLongitude(curpos.longitude());
		}
	}
	else
	{
		prevpos = new QGeoCoordinate(info.coordinate());
	}
	QGeoCoordinate pos = info.coordinate();
}

void QSpeedWidget::SetSpeed(double s)
{
    timer->stop();
    steps = 6;
    setspeed = s;
    delta = (setspeed - curspeed)/steps;
    timer->start(330);
}

void QSpeedWidget::timerStep()
{
	if (steps == 0)
	{
		timer->stop();
		return;
	}

	curspeed += delta;
	steps -= 1;

    if (curspeed > 9)
        scale = 200;

    if (curspeed < 7)
        scale = 10;

    update();
}

void QSpeedWidget::SetDistance(double d)
{
    distance = d;
    update();
}

void QSpeedWidget::SetScale(int s)
{
    scale = s;
    update();
}

void QSpeedWidget::paintEvent(QPaintEvent *)
{
    if ((scale != 10) and (scale !=200))
        return;

    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QPainter painter(this);
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.translate(x,y);
    if (scale == 10)
        painter.drawImage(target, svgSpeed10, source);
    else
        painter.drawImage(target, svgSpeed200, source);

    char buf[10];
    sprintf(buf,"%06.1f",distance);
    painter.setFont(QFont("Courier", h/TEXTDIVIDER));
    QRect r = painter.boundingRect(w/-4,h/3.2,w/2,h/10, Qt::AlignCenter, buf);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    painter.drawRect(r);
    painter.setPen(QPen(Qt::white));
    painter.drawText(w/-4,h/3.2,w/2,h/10, Qt::AlignCenter, buf);

    painter.rotate(360 * curspeed / scale + 180);
    painter.drawImage(target, svgSpeedNeedle, source);
}
