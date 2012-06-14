#include "qgaugewidget.h"
#include "qlevelwidget.h"
#include "datamonitor.h"
#include "ui.h"
#include <QtGui>
#include <QSystemDeviceInfo>

using namespace QtMobility;

QLevelWidget::QLevelWidget(QWidget *parent)
: QGaugeWidget(parent)
{
    SetRange(0,0,1.0);
    SetRange(1,0,100.0);
    SetRange(2,0,200.0);
    SetRange(3,0,200.0);

    connect(this, SIGNAL(longTap()), this, SLOT(Reset()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(1000);
}

void QLevelWidget::SetRange(int id, double minlevel, double maxlevel)
{
	if (minlevel>maxlevel) return;
	if ((id < 0) || (id>3)) return;
	
	min[id] = minlevel;
	max[id] = maxlevel;
	UpdateLevel(id,current[id]);
}

void QLevelWidget::UpdateLevel(int id, double value)
{
	if ((id < 0) || (id > 3)) return;
	
	if ((value>=min[id]) && (value<=max[id]))
		current[id] = value;
	
	if (value<min[id])
		current[id] = min[id];
	
	if (value>max[id])
		current[id] = max[id];
}

void QLevelWidget::Reset()
{
}

void QLevelWidget::paintLevel(int id, QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;
    double angle = (current[id]-min[id])/(max[id]-min[id]) * 70;
    //double angle=60;
    
    QRectF source(0, 0, 360, 180);
    QRectF target(-1*x, -1*y, w, h/2);

    painter.rotate(id *90 - 125 + angle);
    painter.drawImage(target, svgSpeedNeedle, source);
    painter.rotate((id *90 - 125 + angle)*-1);
}

void QLevelWidget::paintPlate(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;
    
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.drawImage(target, svgLevel, source);
}

void QLevelWidget::paintTop(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;
    
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.drawImage(target, svgLevelTop, source);
}

void QLevelWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.translate(width()/2,height()/2);
    paintPlate(painter);
    
    QSystemDeviceInfo info;
    UpdateLevel(0,DataMonitor::Instance().CompassCalibration());
    UpdateLevel(1,info.batteryLevel());
    UpdateLevel(2,DataMonitor::Instance().Vertical());
    UpdateLevel(3,DataMonitor::Instance().Horizontal());
    
    for (int i=0; i<4; i++)
        paintLevel(i,painter);
    
    paintTop(painter);
}
