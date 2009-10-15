#include "qgaugewidget.h"
#include "qspeedwidget.h"
#include "ui.h"
#include <QtGui>

QSpeedWidget::QSpeedWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , scale(10)
    , steps(0)
    , speed(0)
    , delta(0.0)
    , curspeed(0.0)
    , setspeed(0.0)
    , distance(0.0)
{
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerStep()));
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
