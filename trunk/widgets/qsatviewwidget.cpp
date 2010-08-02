#include "qgaugewidget.h"
#include "qsatviewwidget.h"
#include "datamonitor.h"
#include "ui.h"
#include <QtGui>
#include <QGeoSatelliteInfo>

#include <QDebug>
#define LOG( a ) qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

QSatViewWidget::QSatViewWidget(QWidget *parent)
    : QGaugeWidget(parent)
{
    LOG( "QSatViewWidget::QSatViewWidget()"; )
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
    timer->start(5000);
}

void QSatViewWidget::timerExpired()
{
    LOG( "QSatViewWidget::timerExpired()"; )
	update();
}

void QSatViewWidget::paintSatInfo(QPainter &painter, const QGeoSatelliteInfo& info, bool inuse)
{
    LOG3( "QSatViewWidget::paintSatInfo()"; )
    double h = height();
    double s = h / 36;
    double strength = info.signalStrength();
    double azimuth = info.attribute(QGeoSatelliteInfo::Azimuth);
    double elevation = info.attribute(QGeoSatelliteInfo::Elevation);

    if (inuse)
    {
        painter.setPen(Qt::green);
        painter.setBrush(Qt::green);
    }
    else
    {
		int c = (2 * (int(strength/96.0 * 0x7f) % 128)) & 0xff;
            painter.setPen(QColor(c,c,0));
            painter.setBrush(QColor(c,c,0));
    }

    painter.save();
    painter.rotate(azimuth-180);
    painter.translate(0,(90.0 - elevation)/90 * h/2);
    painter.drawEllipse(s/-2,s/-2,s,s);
    painter.restore();
}

void QSatViewWidget::paintEvent(QPaintEvent *)
{
    LOG2( "QSatViewWidget::paintEvent()"; )
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QPainter painter(this);
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.translate(x,y);
    painter.drawImage(target, svgSatView, source);
    
    const QList<QGeoSatelliteInfo>& inview = DataMonitor::Instance().SatsInView();
    const QList<QGeoSatelliteInfo>& inuse = DataMonitor::Instance().SatsInUse();

    for (int i=0; i < inview.length(); i++)
        paintSatInfo(painter, inview[i],false);
    
    for (int i=0; i < inuse.length(); i++)
        paintSatInfo(painter, inuse[i],true);
}
