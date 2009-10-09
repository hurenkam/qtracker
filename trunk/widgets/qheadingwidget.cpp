#include "qheadingwidget.h"
#include "ui.h"
#include <QtGui>

QHeadingWidget::QHeadingWidget(QWidget *parent)
    : QWidget(parent)
    , setdial(0.0)
    , curdial(0.0)
    , deltadial(0.0)
    , stepsdial(0)
    , setneedle(0.0)
    , curneedle(0.0)
    , deltaneedle(0.0)
    , stepsneedle(0)
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerStep()));
    timer->start(330);
}

QHeadingWidget::~QHeadingWidget()
{
}

void QHeadingWidget::timerStep()
{
    if ((stepsdial == 0) && (stepsneedle==0)) return;
    
	if (stepsdial != 0)
	{
	    curdial += deltadial;
	    stepsdial -= 1;
	}
	
	if (stepsdial != 0)
	{
	    curdial += deltadial;
	    stepsdial -= 1;
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
    painter.rotate(curdial);
    painter.drawImage(target, svgHeading, source);
    painter.rotate(-1 * curdial);
    painter.rotate(curneedle);
    painter.drawImage(target, svgCompassNeedle,  source);
}

void QHeadingWidget::SetDial(double v)
{
    double delta = setdial - curdial;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;
    
    setdial = v;
    stepsdial = 6;
    deltadial = delta / stepsdial;
}

void QHeadingWidget::SetNeedle(double v)
{
    double delta = setneedle - curneedle;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;
    
    setneedle = v;
    stepsneedle = 6;
    deltaneedle = delta / stepsneedle;
}

void QHeadingWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        break;
    default:
        break;
    }
}
