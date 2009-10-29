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
#include "QMapWidget.h"
#include "XQLocation.h"
#include "QSignalMapper.h"
#include <QSplashScreen>
#include "ui.h"
#include "math.h"
#include <stdio.h>

const float PI = 3.14159265358979323846f;

#ifdef Q_OS_SYMBIAN
const int STEPCOUNT = 3;
const double STEPCOUNTF = STEPCOUNT;
const int TRANSITTIME = 150;
const int STEPTIME = TRANSITTIME/STEPCOUNT;
#else
const int STEPCOUNT = 7;
const double STEPCOUNTF = STEPCOUNT;
const int TRANSITTIME = 150;
const int STEPTIME = TRANSITTIME/STEPCOUNT;
#endif

int positions[7][6][4] = {
    { {   5,  5,110,110 }, {   5,125,110,110 }, {   5,245,110,110 }, { 525,  5,110,110 }, { 525,125,110,110 }, { 525,245,110,110 } },
    { { 170,  5,350,350 }, {   5,  5,170,170 }, {   5,185,170,170 }, { 525,  5,110,110 }, { 525,125,110,110 }, { 525,245,110,110 } },
    { {   5,  5,170,170 }, { 170,  5,350,350 }, {   5,185,170,170 }, { 525,  5,110,110 }, { 525,125,110,110 }, { 525,245,110,110 } },
    { {   5,  5,170,170 }, {   5,185,170,170 }, { 170,  5,350,350 }, { 525,  5,110,110 }, { 525,125,110,110 }, { 525,245,110,110 } },
    { {   5,  5,110,110 }, {   5,125,110,110 }, {   5,245,110,110 }, { 120,  5,350,350 }, { 465,  5,170,170 }, { 465,185,170,170 } },
    { {   5,  5,110,110 }, {   5,125,110,110 }, {   5,245,110,110 }, { 465,  5,170,170 }, { 120,  5,350,350 }, { 465,185,170,170 } },
    { {   5,  5,110,110 }, {   5,125,110,110 }, {   5,245,110,110 }, { 465,  5,170,170 }, { 465,185,170,170 }, { 120,  5,350,350 } },
};

int intermediate[6][4];

static QRect GetStepRect(QRect from, QRect to, int step)
{
    QRect result(0,0,0,0);

    int dx = to.x() - from.x();
    int dy = to.y() - from.y();
    int dw = to.width() - from.width();
    int dh = to.height() - from.height();

    result.setX      ( step / STEPCOUNTF * dx + from.x() );
    result.setY      ( step / STEPCOUNTF * dy + from.y() );
    result.setWidth  ( step / STEPCOUNTF * dw + from.width() );
    result.setHeight ( step / STEPCOUNTF * dh + from.height() );

    return result;
}

static QRect GetIntermediateRect(bool landscape, int gauge, int tozoom, int fromzoom, int step)
{
    int x = 0;
    int y = 1;
    int w = 2;
    int h = 3;

    if (!landscape)
    {
        x = 1;
        y = 0;
        w = 3;
        h = 2;
    }

    QRect to = QRect(
            positions[tozoom][gauge][x],
            positions[tozoom][gauge][y],
            positions[tozoom][gauge][w],
            positions[tozoom][gauge][h]
        );
    QRect from = QRect(
            positions[fromzoom][gauge][x],
            positions[fromzoom][gauge][y],
            positions[fromzoom][gauge][w],
            positions[fromzoom][gauge][h]
        );
    return GetStepRect(from,to,step);
}

static QRect GetRect(bool landscape,int gauge,int zoom)
{
    if (landscape)
        return QRect(
                positions[zoom][gauge][0],
                positions[zoom][gauge][1],
                positions[zoom][gauge][2],
                positions[zoom][gauge][3]
            );
    else
        return QRect(
                positions[zoom][gauge][1],
                positions[zoom][gauge][0],
                positions[zoom][gauge][3],
                positions[zoom][gauge][2]
            );
}

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
        , zoomstep(0)
        , zoomgauge(0)
        , tozoom(0)
        , landscape(true)
        , starttime(0,0,0)
        , distance(0)
        , timevalid(false)
        , posvalid(false)
        , showmap(true)
        , mapzoomed(false)
{
}

void QDashWindow::Init(QSplashScreen *splash)
{
    splash->showMessage("Loading images...",Qt::AlignLeft);
    LoadImages();
    splash->showMessage("Initialising widgets...",Qt::AlignLeft);
    InitWidgets();
    splash->showMessage("Setting up main screen...",Qt::AlignLeft);

    #ifdef Q_OS_SYMBIAN
    showFullScreen();
    #else
    //resize(360,640);
    resize(640,360);
    #endif

    zoomtimer = new QTimer(this);
    connect(zoomtimer, SIGNAL(timeout()), this, SLOT(ZoomTimerExpired()));

    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(timeChanged()));
    connect(&location, SIGNAL(locationChanged(double, double, double, float, float)), this, SLOT(locationChanged(double, double, double, float, float)));
    connect(&location, SIGNAL(updateSatInfo(int,int,double,double,bool)), this, SLOT(updateSatInfo(int,int,double,double,bool)));

    connect(&location, SIGNAL(altitudeChanged(double,float)), this, SLOT(updateAltitude(double)));
    connect(&location, SIGNAL(speedChanged(float)), this, SLOT(updateSpeed(float)));
    connect(&location, SIGNAL(headingChanged(float)), this, SLOT(updateHeading(float)));

    connect(speed, SIGNAL(longTap()), this, SLOT(resetDistance()));
    connect(timer, SIGNAL(longTap()), this, SLOT(resetTimer()));
    connect(map, SIGNAL(longTap()), this, SLOT(ToggleMap()));

    t->start(1000);
    if (location.open() == XQLocation::NoError)
    {
        location.startUpdates();
        heading->SetDial(0);
        heading->SetNeedle(0);
    }
    else
    {
        heading->SetDial(45);
        heading->SetNeedle(-45);
    }

    QFile file(UIDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);
}

void QDashWindow::InitWidgets()
{
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

    map = new QMapWidget(this);
    map->setObjectName(QString::fromUtf8("heading"));
    map->setGeometry(QRect(120, 0, 400, 360));

    gauges[0] = heading;
    gauges[1] = clock;
    gauges[2] = speed;
    gauges[3] = satview;
    gauges[4] = timer;
    gauges[5] = altitude;

    mapper = new QSignalMapper(this);
    for (int i=0; i<6; i++)
    {
        connect(gauges[i], SIGNAL(singleTap()), mapper, SLOT(map()));
        mapper->setMapping(gauges[i], i+1);
    }
    connect(mapper,SIGNAL(mapped(const int &)),this,SLOT(ZoomToGauge(const int &)));
}

void QDashWindow::resetTimer()
{
    timevalid = false;
    timeChanged();
}

void QDashWindow::timeChanged()
{
    if (zoomstep != 0) return;

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
    if (zoomstep != 0) return;

    altitude->SetAltitude(alt);
}

void QDashWindow::resetDistance()
{
    distance = 0;
    speed->SetDistance(distance);
}

void QDashWindow::updateDistance(double lat, double lon)
{
    if (zoomstep != 0) return;

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

void QDashWindow::updateHeading(float course)
{
    if (zoomstep != 0) return;

    heading->SetDial(360-course);
    heading->SetNeedle(0);
}

void QDashWindow::updateSpeed(float s)
{
    if (zoomstep != 0) return;

    speed->SetSpeed(s*3.6);
}

void QDashWindow::updateSatInfo(int id, int strength, double azimuth, double elevation, bool inuse)
{
    if (zoomstep != 0) return;

    satview->SetSatInfo(id,strength,azimuth,elevation,inuse);
}

void QDashWindow::locationChanged(
    double lat,
    double lon,
    double alt,
    float s,
    float course)
{
    if (zoomstep != 0) return;

    updateDistance(lat,lon);
    if (zoomgauge == 0)
        map->updatePosition(lat,lon);
}

void QDashWindow::ToggleMap()
{
    mapzoomed = !mapzoomed;
    Setup();
    update();
}

void QDashWindow::Setup()
{
    if (mapzoomed)
    {
        for (int i =0; i<6; i++)
            gauges[i]->hide();

        map->setGeometry(0,0,width(),height());
        return;
    }

    if (zoomstep == 0)
    {
        for (int i = 0; i<6; i++)
        {
            gauges[i]->setGeometry(GetRect(landscape,i,zoomgauge));
            gauges[i]->show();
        }
    }
    else
    {
        QRect r;
        for (int i = 0; i<6; i++)
        {
            r = GetIntermediateRect(landscape,i,zoomgauge,tozoom,zoomstep);
            gauges[i]->setGeometry(r);
            gauges[i]->show();
        }
    }

    if ( zoomstep == 0)             // No transition
    {
        if (zoomgauge != 0)         // Gauge zoomed
            //map->setGeometry(0,0,0,0);
            map->hide();
        else                        // Map visibile
        {
            if (landscape)          // Landscape
                map->setGeometry(120,0,400,360);
            else                    // Portrait
                map->setGeometry(0,120,360,400);
            map->show();
        }
    }
    else                            // In Transition
    {
        int delta = int(zoomstep/STEPCOUNTF * 360);
        if (tozoom == 0)
        {
            if (landscape)
                map->setGeometry(120,0+delta,400,360+delta);
            else
                map->setGeometry(0+delta,120,360+delta,400);
            map->show();
        }
        if (zoomgauge == 0)
        {
            if (landscape)
                map->setGeometry(120,360-delta,400,720-delta);
            else
                map->setGeometry(360-delta,120,720-delta,400);
            map->show();
        }
    }
}

void QDashWindow::StartTransition(int to)
{
    if (zoomgauge != to)
        tozoom = to;
    else
        if (zoomgauge == 0)
            return;
        else
            tozoom = 0;

    zoomstep = STEPCOUNT -1;
    Setup();
    update();
    zoomtimer->start(STEPTIME);
}

void QDashWindow::ZoomToGauge(int i)
{
    // return if in transition
    if (zoomstep > 0) return;
    StartTransition(i);
}

void QDashWindow::GaugeOptions(int /*i*/)
{
    // return if in transition
    if (zoomstep > 0) return;
}

void QDashWindow::ZoomTimerExpired()
{
    if (zoomstep > 0)
    {
        Setup();
        update();
        zoomstep -= 1;
    }
    else
    {
        zoomgauge = tozoom;
        zoomtimer->stop();
        Setup();
        update();
    }
}

void QDashWindow::resizeEvent ( QResizeEvent * event )
{
    landscape = (width() > height());
    Setup();
    QMainWindow::resizeEvent(event);
}

QDashWindow::~QDashWindow()
{
}
