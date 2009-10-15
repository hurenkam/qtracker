#include <QtGui>
#include "qgaugewidget.h"
#include <iostream.h>
/*
static const int QGaugeWidget::StIdle = 0;
static const int QGaugeWidget::StInitialTouch = 1;
static const int QGaugeWidget::StSingleTapPending = 2;
static const int QGaugeWidget::StLongTouch = 3;
static const int QGaugeWidget::StSecondTouch = 4;
static const int QGaugeWidget::StMoving = 5;
*/
QGaugeWidget::QGaugeWidget(QWidget *parent)
    : QWidget(parent)
    //, longpress(false)
    //, timer(0)
{
    //timer = new QTimer(this);
    //timer->setSingleShot(true);
    //timer->setInterval(1000);
    //connect(timer,SIGNAL(timeout()),this,SLOT(timerExpired()));

	singletaptimer.setSingleShot(true);
	singletaptimer.setInterval(300);
	connect(&singletaptimer,SIGNAL(timeout()),this,SLOT(singleTapTimeout()));
	longtaptimer.setSingleShot(true);
	longtaptimer.setInterval(1000);
	connect(&longtaptimer,SIGNAL(timeout()),this,SLOT(longTapTimeout()));
}

QGaugeWidget::~QGaugeWidget()
{
    //cancelTimer();
    //delete timer;
    //timer =0;
}
/*
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
*/
void QGaugeWidget::mousePressEvent(QMouseEvent * /* event */)
{
    //cout << "Gauge mouse press\n";
    //longpress = false;
    //startTimer();

	switch(state)
	{
	case StIdle:
		startLongTapTimer();
		state = StInitialTouch;
		break;
	case StSingleTapPending:
		cancelSingleTapTimer();
		startLongTapTimer();
		state = StSecondTouch;
		break;
	default:
		// error!
		break;
	}
}

void QGaugeWidget::longTapTimeout()
{
	switch(state)
	{
	case StInitialTouch:
	case StSecondTouch:
		state = StLongTouch;
		break;
	default:
		// error!
		break;
	}
}

void QGaugeWidget::singleTapTimeout()
{
	switch(state)
	{
	case StSingleTapPending:
		state = StIdle;
		emit singleTap();
		break;
	default:
		// error!
		break;
	}
}

void QGaugeWidget::startLongTapTimer()
{
    longtaptimer.start();
}

void QGaugeWidget::startSingleTapTimer()
{
	singletaptimer.start();
}

void QGaugeWidget::cancelLongTapTimer()
{
	longtaptimer.stop();
}

void QGaugeWidget::cancelSingleTapTimer()
{
	singletaptimer.stop();
}

void QGaugeWidget::mouseMoveEvent(QMouseEvent *event)
{
    // if (movement not too small) emit drag event
}

void QGaugeWidget::mouseReleaseEvent(QMouseEvent * /*event*/)
{
/*
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
*/
	switch(state)
	{
	case QGaugeWidget::StInitialTouch:
		cancelLongTapTimer();
		state = StSingleTapPending;
		break;
	case QGaugeWidget::StSecondTouch:
		cancelLongTapTimer();
		state = StIdle;
		emit doubleTap();
		break;
	case QGaugeWidget::StLongTouch:
		state = StIdle;
		emit longTap();
		break;
	case QGaugeWidget::StMoving:
		// emit drag event
		break;
	default:
		// error!
		break;
	}
}
