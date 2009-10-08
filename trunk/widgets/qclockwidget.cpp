#include "qclockwidget.h"
#include "ui.h"
#include <QtGui>

QClockWidget::QClockWidget(QWidget *parent)
    : QWidget(parent)
    , hours(0)
    , minutes(0)
    , seconds(0)
{
}

void QClockWidget::SetTime(int h,int m,int s)
{
    hours = h;
    minutes = m;
    seconds = s;
    update();
}

QClockWidget::~QClockWidget()
{
}

void QClockWidget::paintEvent(QPaintEvent *)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;

    QPainter painter(this);
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.translate(x,y);
    painter.drawImage(target, svgClock, source);
    painter.rotate(hours*30 + minutes/2);
    painter.drawImage(target, svgShort, source);
    painter.rotate(-1 * (hours*30 + minutes/2));
    painter.rotate(minutes*6);
    painter.drawImage(target, svgLong, source);
    painter.rotate(-1 * minutes*6);
    painter.rotate(seconds*6);
    painter.drawImage(target, svgSecond, source);
}

void QClockWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        break;
    default:
        break;
    }
}
