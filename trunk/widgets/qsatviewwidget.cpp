#include "qsatviewwidget.h"
#include "ui.h"
#include <QtGui>

QSatViewWidget::QSatViewWidget(QWidget *parent)
    : QWidget(parent)
{
    ClearSatInfo(-1);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired));
	timer->start(5000);
}

QSatViewWidget::~QSatViewWidget()
{
}

void QSatViewWidget::timerExpired()
{
	ClearSatInfo(-1);
}

void QSatViewWidget::ClearSatInfo(int id)
{
    if ((id < -1) || (id > 31)) return;
    
    if (id == -1)
    {
        for (id = 0; id < 31; id++)
        	strength[id] = -1;
    }
    else
    	strength[id] = -1;
}

//void QSatViewWidget::SetSatInfo(XQSatInfo &satinfo)
void QSatViewWidget::SetSatInfo(int i, int s, double a, double e, bool u)
{
	if ((i <0) || (i>31)) return;
	
    inuse[i] = u;
    strength[i] = s;
    azimuth[i] = a;
    elevation[i] = e;
}

void QSatViewWidget::paintSatInfo(QPainter &painter, int id)
{
    double h = height();
    double s = h / 36;
    
    if (inuse[id])
    {
    	painter.setPen(Qt::green);
    	painter.setBrush(Qt::green);
    }
    else
	{
        int c = 2 * (int(strength[id]/64.0 * 0x7f) % 128);
		painter.setPen(QColor(c,c,0));
		painter.setBrush(QColor(c,c,0));
	}
    
	painter.save();
	painter.rotate(azimuth[id]-180);
	painter.translate(0,(90.0 - elevation[id])/90 * h/2);
	painter.drawEllipse(s/-2,s/-2,s,s);
	painter.restore();
}

void QSatViewWidget::paintEvent(QPaintEvent *)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QPainter painter(this);
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.translate(x,y);
    painter.drawImage(target, svgSatView, source);
    
    for (int i=0; i < 31; i++)
        paintSatInfo(painter, i);
}

void QSatViewWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        break;
    default:
        break;
    }
}
