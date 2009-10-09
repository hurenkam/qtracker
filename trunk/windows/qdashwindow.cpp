/*
 * QDashWindow.cpp
 *
 *  Created on: 4-okt-2009
 *      Author: Mark
 */

#include <QtGui>
#include "QDashWindow.h"
#include "QClockWidget.h"
#include "QSpeedWidget.h"
#include "QAltitudeWidget.h"
#include "QSatViewWidget.h"
#include "QHeadingWidget.h"
#include "XQLocation.h"
#include "ui.h"
#include "math.h"

//#define PI 3.14159265
const float PI = 3.14159265358979323846f;

static void CalculateDistanceAndBearing(
		double fromlat,
		double fromlon,
		double tolat,
		double tolon,
		double &distance,
		double &bearing)
{
    double earths_radius = (6378137.0 + 6356752.3141) / 2.0;

    double from_theta = float(fromlat) / 360.0 * 2.0 * PI;
    double from_landa = float(fromlon) / 360.0 * 2.0 * PI;
    double to_theta = float(tolat)     / 360.0 * 2.0 * PI;
    double to_landa = float(tolon)     / 360.0 * 2.0 * PI;
    
    distance = acos(
            sin(from_theta) * sin(to_theta) +
            cos(from_theta) * cos(to_theta) * cos(to_landa-from_landa)
                ) * earths_radius;
    
    bearing = atan2(
                sin(to_landa-from_landa) * cos(to_theta),
                cos(from_theta) * sin(to_theta) -
                sin(from_theta) * cos(to_theta) * cos(to_landa-from_landa)
            );
    bearing = bearing / 2.0 / PI * 360.0;
}

QDashWindow::QDashWindow(QWidget *parent)
	: QMainWindow(parent)
	, starttime(0,0,0)
	, counter(0)
	, altvalid(false)
	, timevalid(false)
	, posvalid(false)
	, showmap(false)
	, distance(0)
{
    LoadImages();
    clock = new QClockWidget(this);
    clock->setObjectName(QString::fromUtf8("clock"));
    clock->setGeometry(QRect(5, 5, 170, 170));
    speed = new QSpeedWidget(this);
    speed->setObjectName(QString::fromUtf8("speed"));
    speed->setGeometry(QRect(5, 185, 170, 170));
    timer = new QClockWidget(this);
    timer->setObjectName(QString::fromUtf8("timer"));
    timer->setGeometry(QRect(525, 5, 110, 110));
    altitude = new QAltitudeWidget(this);
    altitude->setObjectName(QString::fromUtf8("altitude"));
    altitude->setGeometry(QRect(525, 125, 110, 110));
    satview = new QSatViewWidget(this);
    satview->setObjectName(QString::fromUtf8("satview"));
    satview->setGeometry(QRect(525, 245, 110, 110));
    heading = new QHeadingWidget(this);
    heading->setObjectName(QString::fromUtf8("heading"));
    heading->setGeometry(QRect(170, 5, 350, 350));

    //QFile file("!:/private/ea82cef3/style.css");
    QFile file(UIDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);

    #ifdef Q_OS_SYMBIAN
    showFullScreen();
    #else
    resize(640,360);
    #endif
	
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeChanged()));
    connect(&location, SIGNAL(locationChanged(double, double, double, float, float)), this, SLOT(locationChanged(double, double, double, float, float)));
    connect(&location, SIGNAL(updateSatInfo(int,int,double,double,bool)), this, SLOT(updateSatInfo(int,int,double,double,bool)));
    
    timer->start(1000);
    if (location.open() == XQLocation::NoError)
    {
        location.startUpdates();
        heading->SetDial(0);
        heading->SetNeedle(0);
    }
    else
    {
        heading->SetDial(0);
        heading->SetNeedle(90);
    }
}

void QDashWindow::timeChanged()
{
    QTime time = QTime::currentTime();
    int second = time.second();
    int minute = time.minute();
    int hour = time.hour();
    clock->SetTime(hour,minute,second);

    if (timevalid)
    {
        hour   -= starttime.hour();
        minute -= starttime.minute();
        second -= starttime.second();
        if (second < 0)
        {
            second += 60;
            minute -= 1;
        }
        if (minute < 0)
        {
            minute += 60;
            hour -= 1;
        }
        timer->SetTime(hour,minute,second);
    }
    else
    {
        timevalid = true;
        starttime = time;
    }
}

void QDashWindow::updateAltitude(double alt)
{
	altitude->SetAltitude(alt);
}

void QDashWindow::updateDistance(double lat, double lon)
{
	if (posvalid)
	{
		double d = 0;
		double b = 0;
		CalculateDistanceAndBearing(prevlat,prevlon,lat,lon,d,b);
		if (d > 25)
		{
			distance += d/1000.0;
			prevlat = lat;
			prevlon = lon;
			speed->SetDistance(distance);
		}
	}
	else
	{
		posvalid = true;
		prevlat = lat;
		prevlon = lon;
	}
}

void QDashWindow::updateHeading(double course)
{
	heading->SetDial(360-course);
	heading->SetNeedle(0);
}

void QDashWindow::updateSpeed(double s)
{
	speed->SetSpeed(s*3.6);
}

void QDashWindow::updateSatInfo(int id, int strength, double azimuth, double elevation, bool inuse)
{
	satview->SetSatInfo(id,strength,azimuth,elevation,inuse);
}

void QDashWindow::locationChanged(
		double lat, 
		double lon, 
		double alt, 
		float s, 
		float course)
{
	updateDistance(lat,lon);
	updateHeading(course);
	updateSpeed(s);
	updateAltitude(alt);
	satview->update();
}

void QDashWindow::Setup640x360()
{
    if (showmap)
    {
		heading->setGeometry(QRect(5, 5, 110, 110));
		clock->setGeometry(QRect(5, 125, 110, 110));
		speed->setGeometry(QRect(5, 245, 110, 110));
    }
    else
    {
		clock->setGeometry(QRect(5, 5, 170, 170));
		speed->setGeometry(QRect(5, 185, 170, 170));
		heading->setGeometry(QRect(170, 5, 350, 350));
    }
	satview->setGeometry(QRect(525, 5, 110, 110));
	timer->setGeometry(QRect(525, 125, 110, 110));
	altitude->setGeometry(QRect(525, 245, 110, 110));
}

void QDashWindow::Setup360x640()
{
	if (showmap)
	{
		satview->setGeometry(QRect(245, 5, 110, 110));
		clock->setGeometry(QRect(125, 5, 110, 110));
		heading->setGeometry(QRect(5, 5, 110, 110));
	}
	else
	{
		clock->setGeometry(QRect(185, 5, 170, 170));
		satview->setGeometry(QRect(5, 5, 170, 170));
		heading->setGeometry(QRect(5, 170, 350, 350));
	}
	altitude->setGeometry(QRect(245, 525, 110, 110));
	timer->setGeometry(QRect(125, 525, 110, 110));
	speed->setGeometry(QRect(5, 525, 110, 110));
}

void QDashWindow::resizeEvent ( QResizeEvent * event )
{
	if ((width() == 640) && (height() == 360))
		Setup640x360();
	if ((width() == 360) && (height() == 640))
		Setup360x640();
	
	QMainWindow::resizeEvent(event);
}

QDashWindow::~QDashWindow()
{
}
