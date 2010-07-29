#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QRect>
#include "QDatumWgs84.h"

QWgs84DisplayWidget::QWgs84DisplayWidget(QWidget *parent)
: QDatumDisplayWidget(parent)
{}

void QWgs84DisplayWidget::paintEvent(QPaintEvent *event)
{
	QDatumDisplayWidget::paintEvent(event);
    QPainter painter(this);
	
	double w = width();
	double h = height();

    if ( source & 1 )
	    painter.setPen(QPen(Qt::blue));
	else if ( source & 2 )
	    painter.setPen(QPen(Qt::black));
	else // unknown
	    painter.setPen(QPen(Qt::gray));
	    
    char buf[25];
	sprintf(buf,"%08.5fN %08.5fE",latitude,longitude);
	
	QRect r = painter.boundingRect(0,0,w,h, Qt::AlignLeft, buf);
	painter.drawText(r, Qt::AlignLeft, buf);
}

QWgs84EditWidget::QWgs84EditWidget(QWidget *parent)
: QDatumEditWidget(parent)
{}
