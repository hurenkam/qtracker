#include <QWidget>
#include <QSvgRenderer>
#include "qheadingwidget.h"
#include "ui.h"

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
    timer = new QTimer(this);
    svgdial =   new QSvgRenderer(QString(UIDIR "compass.svg"), this);
    svgneedle = new QSvgRenderer(QString(UIDIR "compassneedle.svg"), this);

    connect(timer, SIGNAL(timeout()), this, SLOT(timerStep()));
    connect(svgdial, SIGNAL(repaintNeeded()), this, SLOT(update()));
    connect(svgneedle, SIGNAL(repaintNeeded()), this, SLOT(update()));
}

QHeadingWidget::~QHeadingWidget()
{
}

void QHeadingWidget::timerStep()
{
    if ((stepsdial == 0) && (stepsneedle==0))
    {
        timer->stop();
        return;
    }

    if (stepsdial != 0)
    {
        curdial += deltadial;
        stepsdial -= 1;
    }

    if (stepsneedle != 0)
    {
        curneedle += deltaneedle;
        stepsneedle -= 1;
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

    svgdial->render(&painter,target);
    painter.rotate(-1 * curdial);
    painter.rotate(curneedle);
    svgneedle->render(&painter,target);
}

void QHeadingWidget::SetDial(double v)
{
    timer->stop();

    double delta = setdial - curdial;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    setdial = v;
    stepsdial = 6;
    deltadial = delta / stepsdial;

    timer->start(330);
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
