#include "qgaugewidget.h"
#include "qclockwidget.h"
#include "qclockdialog.h"
#include "datamonitor.h"
#include "ui.h"
#include <QtGui>

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

QClockWidget::QClockWidget(QWidget *parent)
: QGaugeWidget(parent)
, remainingtime(0)
{
	ReadSettings();
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(this, SIGNAL(longTap()), this, SLOT(Reset()));
	connect(&DataMonitor::Instance(), SIGNAL(TimeUpdated(long)), this, SLOT(SetRemainingTime(long)));
    connect(this, SIGNAL(doubleTap()), this, SLOT(SelectOptions()));
    
	timer.start(1000);
}

void QClockWidget::SelectOptions()
{
    LOG( "QClockWidget::SelectOptions()"; )
	QClockDialog *dialog;
	dialog = new QClockDialog(this);
	dialog->setModal(true);
	connect(dialog,SIGNAL(accepted()),this,SLOT(ReadSettings()));
	dialog->show();
}

void QClockWidget::ReadSettings()
{
	starttime =     settings.value("clock/starttime",QDateTime::fromTime_t(0)).toDateTime();
	analog = (Type) settings.value("clock/analog",0).toInt();
	top    = (Type) settings.value("clock/top",1).toInt();
	bottom = (Type) settings.value("clock/bottom",3).toInt();
}

QTime QClockWidget::GetTime(Type t)
{
	switch (t)
	{
		case CurrentTime: return QDateTime::currentDateTime().time();
		case TripTime:
		{
		    int secs = starttime.secsTo(QDateTime::currentDateTime());
		    return QTime(0,0,0).addSecs(secs);
		}
		case RemainingTime:
		{
			return QTime(0,0,0).addSecs(remainingtime);
		}
		case ArrivalTime:
		{
			return QDateTime::currentDateTime().time().addSecs(remainingtime);
		}
	}
}

void QClockWidget::Reset()
{
	starttime = QDateTime::currentDateTime();
	settings.setValue("clock/starttime",starttime);
	settings.sync();
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

void QClockWidget::paintAnalog(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);
    
    QTime time = GetTime(analog);
    int hours = time.hour();
    int minutes = time.minute();
    int seconds = time.second();

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

void QClockWidget::paintTop(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    QTime time = GetTime(top);

    painter.setFont(QFont("Courier", h/TEXTDIVIDER));
    QRect r = painter.boundingRect(w/-4,h/6,w/2,h/12, Qt::AlignCenter, time.toString("hh:mm:ss"));
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    painter.drawRect(r);
    painter.setPen(QPen(Qt::white));
    painter.drawText(w/-4,h/6,w/2,h/12, Qt::AlignCenter, time.toString("hh:mm:ss"));
}

void QClockWidget::paintBottom(QPainter& painter)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    QTime time = GetTime(bottom);
    
    painter.setFont(QFont("Courier", h/TEXTDIVIDER));
    QRect r = painter.boundingRect(w/-4,h/3.5,w/2,h/12, Qt::AlignCenter, time.toString("hh:mm:ss"));
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    painter.drawRect(r);
    painter.setPen(QPen(Qt::white));
    painter.drawText(w/-4,h/3.5,w/2,h/12, Qt::AlignCenter, time.toString("hh:mm:ss"));
}

void QClockWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    paintPlate(painter);
    paintTop(painter);
    paintBottom(painter);
    paintAnalog(painter);
}
