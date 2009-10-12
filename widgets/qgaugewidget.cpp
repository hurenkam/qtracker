#include <QtGui>
#include "qgaugewidget.h"
#include <iostream.h>

QGaugeWidget::QGaugeWidget(QWidget *parent)
    : QWidget(parent)
    , longpress(false)
    , timer(0)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerExpired()));
}

QGaugeWidget::~QGaugeWidget()
{
    cancelTimer();
    delete timer;
    timer =0;
}

void QGaugeWidget::startTimer()
{
    //cout << "Gauge timer started\n";
    timer->start();
}

void QGaugeWidget::cancelTimer()
{
    timer->stop();
}

void QGaugeWidget::timerExpired()
{
    //cout << "Gauge timer expired\n";
    longpress = true;
}

void QGaugeWidget::mousePressEvent(QMouseEvent *event)
{
    //cout << "Gauge mouse press\n";
    longpress = false;
    startTimer();
}

void QGaugeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //cout << "Gauge mouse release\n";
    if (longpress)
    {
        //cout << "Gauge emit options()\n";
        emit options();
    }
    else
    {
        //cout << "Gauge emit zoom()\n";
        emit zoom();
    }
    longpress = false;
}
