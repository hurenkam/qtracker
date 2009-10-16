#include <QtGui>
#include "qgaugewidget.h"
#include "qaltitudewidget.h"
#include "ui.h"

QAltitudeWidget::QAltitudeWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , current(0)
    , set(0)
    , delta(0)
    , steps(-1)
    , min(0.2)
    , max(0.1)
    , valid(false)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerStep()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(reset()));
}

void QAltitudeWidget::timerStep()
{
        if (steps == 0)
        {
                if (min > max)
                {
                        min = current;
                        max = current;
                        update();
                }
                if (current < min)
                {
                        min = current;
                        update();
                }
                if (current > max)
                {
                        max = current;
                        update();
                }
        timer->stop();
        }
        else if (steps > 0)
        {
                current += delta;
                steps -= 1;

                update();
        }
}

void QAltitudeWidget::reset()
{
    min = 0.2;
    max = 0.1;
    valid = false;
    update();
}

void QAltitudeWidget::SetAltitude(double alt)
{
        if (set == alt) return;
        timer->stop();

    set = alt;
    steps = 6;
    delta = (set-current)/steps;

    timer->start(330);
}

void QAltitudeWidget::paintEvent(QPaintEvent *)
{
    double w = width();
    double x = w/2;
    double h = height();
    double y = h/2;
    char buf[10];

    QPainter painter(this);
    QRectF source(0, 0, 360, 360);
    QRectF target(-1*x, -1*y, w, h);

    painter.translate(x,y);
    painter.drawImage(target, svgAltitude, source);

    painter.setFont(QFont("Courier", h/TEXTDIVIDER));
    sprintf(buf,"%5.5i",int(min));
    QRect r = painter.boundingRect(w/-4,h/3,w/2,h/12, Qt::AlignCenter, buf);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    painter.drawRect(r);
    painter.setPen(QPen(Qt::white));
    painter.drawText(w/-4,h/3,w/2,h/12, Qt::AlignCenter, buf);

    sprintf(buf,"%5.5i",int(max));
    r = painter.boundingRect(w/-4,h/5.5,w/2,h/12, Qt::AlignCenter, buf);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    painter.drawRect(r);
    painter.setPen(QPen(Qt::white));
    painter.drawText(w/-4,h/5.5,w/2,h/12, Qt::AlignCenter, buf);

    double s = current / 1000;
    double l = current - int(s)*1000;

    painter.save();
    painter.rotate(s * 36 + 180);
    painter.drawImage(target, svgShort, source);
    painter.restore();
    painter.rotate(l/1000 * 360 + 180);
    painter.drawImage(target, svgLong, source);
}
