#include "qgaugewidget.h"
#include "qclockwidget.h"
#include "datamonitor.h"
#include "ui.h"
#include <QtGui>

QClockWidget::QClockWidget(QWidget *parent)
: QGaugeWidget(parent)
, starttime (settings.value("dash/starttime",(0,0,0)).toTime())
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(TimeChanged()));
    connect(this, SIGNAL(longTap()), this, SLOT(Reset()));
	connect(&DataMonitor::Instance(), SIGNAL(TimeUpdated(const QTime&)), this, SLOT(SetTime(const QTime&)));
	timer.start(1000);
	top = QTime(0,0,0);
}

void QClockWidget::SetTime(const QTime& time)
{
	top = time;
	update();
}

void QClockWidget::TimeChanged()
{
	analog = QTime::currentTime();
	bottom = QTime(0,0,0).addSecs(starttime.secsTo(analog));
	update();
}

void QClockWidget::Reset()
{
	starttime = QTime::currentTime();
	settings.setValue("dash/starttime",starttime);
	settings.sync();
    TimeChanged();
}

void QClockWidget::paintPlate(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;
    
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.translate(x,y);
    painter.drawImage(target, svgClock, source);
}

void QClockWidget::paintTop(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.setFont(QFont("Courier", h/TEXTDIVIDER));
    QRect r = painter.boundingRect(w/-4,h/6,w/2,h/12, Qt::AlignCenter, top.toString("hh:mm:ss"));
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    painter.drawRect(r);
    painter.setPen(QPen(Qt::white));
    painter.drawText(w/-4,h/6,w/2,h/12, Qt::AlignCenter, top.toString("hh:mm:ss"));
}

void QClockWidget::paintBottom(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.setFont(QFont("Courier", h/TEXTDIVIDER));
    QRect r = painter.boundingRect(w/-4,h/3.5,w/2,h/12, Qt::AlignCenter, bottom.toString("hh:mm:ss"));
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    painter.drawRect(r);
    painter.setPen(QPen(Qt::white));
    painter.drawText(w/-4,h/3.5,w/2,h/12, Qt::AlignCenter, bottom.toString("hh:mm:ss"));
}

void QClockWidget::paintAnalog(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);
    
    int hours = analog.hour();
    int minutes = analog.minute();
    int seconds = analog.second();

    painter.rotate(hours*30 + minutes/2);
    painter.drawImage(target, svgShort, source);
    painter.rotate(-1 * (hours*30 + minutes/2));
    painter.rotate(minutes*6);
    painter.drawImage(target, svgLong, source);
    painter.rotate(-1 * minutes*6);
    painter.rotate(seconds*6);
    painter.drawImage(target, svgSecond, source);
    painter.rotate(-1 * seconds*6);
}

void QClockWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    paintPlate(painter);
    paintTop(painter);
    paintBottom(painter);
    paintAnalog(painter);
}
