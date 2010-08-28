/*
 * QDashWindow.cpp
 *
 *  Created on: 4-okt-2009
 *      Author: Mark
 */

#include <QtGui>
#include <QSplashScreen>
#include "qdashwindow.h"
#include "qclockwidget.h"
#include "qlevelwidget.h"
#include "qspeedwidget.h"
#include "qaltitudewidget.h"
#include "qsatviewwidget.h"
#include "qheadingwidget.h"
#include "qmapwidget.h"
#include "qmapstatusbar.h"
#include "qsignalmapper.h"
#include "ui.h"

//#include <QDebug>
//#define LOG( a ) qDebug() << a
#define LOG( a ) 


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
    { {   0,  0,120,120 }, {   0,120,120,120 }, {   0,240,120,120 }, { 520,  0,120,120 }, { 520,120,120,120 }, { 520,240,120,120 } },
    { { 175,  0,360,360 }, {   0,  0,180,180 }, {   0,180,180,180 }, { 520,  0,120,120 }, { 540,130,100,100 }, { 520,240,120,120 } },
    { {   0,  0,180,180 }, { 175,  0,360,360 }, {   0,180,180,180 }, { 520,  0,120,120 }, { 540,130,100,100 }, { 520,240,120,120 } },
    { {   0,  0,180,180 }, {   0,180,180,180 }, { 175,  0,360,360 }, { 520,  0,120,120 }, { 540,130,100,100 }, { 520,240,120,120 } },
    { {   0,  0,120,120 }, {   0,130,100,100 }, {   0,240,120,120 }, { 110,  0,360,360 }, { 460,  0,180,180 }, { 460,180,180,180 } },
    { {   0,  0,120,120 }, {   0,130,100,100 }, {   0,240,120,120 }, { 460,  0,180,180 }, { 110,  0,360,360 }, { 460,180,180,180 } },
    { {   0,  0,120,120 }, {   0,130,100,100 }, {   0,240,120,120 }, { 460,  0,180,180 }, { 460,180,180,180 }, { 110,  0,360,360 } },
};
int buttonpositions[8][2] = {
//      waypoint      route        track       zoom-in      zoom-out      menu          exit       statusbar
	{  10, 10 }, {  60, 10 }, { 110, 10 }, { -60, 10 }, { -60, 60 }, {  10, -60 }, { -60,-60 }, {  45,-55 }
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

QDashWindow::QDashWindow(QSettings& s, QWidget *parent)
        : QMainWindow(parent)
        , zoomstep    (0)
        , zoomgauge   (s.value("dash/zoomgauge",0).toInt())
        , tozoom      (0)
        , landscape   (true)
        , mapzoomed   (s.value("dash/mapzoomed",false).toBool())
        , settings(s)
{
}

void QDashWindow::Init(QSplashScreen *splash)
{
    splash->showMessage("Loading images...",Qt::AlignLeft);
    LoadImages();
    splash->showMessage("Initialising widgets...",Qt::AlignLeft);
    InitWidgets();
    splash->showMessage("Setting up main screen...",Qt::AlignLeft);
    showFullScreen();

    zoomtimer = new QTimer(this);
    connect(zoomtimer, SIGNAL(timeout()), this, SLOT(ZoomTimerExpired()));
    //connect(timer, SIGNAL(longTap()), this, SLOT(resetTimer()));
    connect(map, SIGNAL(longTap()), this, SLOT(ToggleMap()));
    
    QFile file(SPLASHRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);
}

QToolButton* QDashWindow::PlaceButton(QString name, QWidget* group, bool repeat)
{    
	//if (pos[0] < 0) pos[0] += width();
	//if (pos[0] < 0) pos[0] += height();
	
    QToolButton* button = new QToolButton(group);
    button->setGeometry(QRect(0,0, 50, 50));
    QIcon icon;
    icon.addFile(QString(MAPRCDIR) % name, QSize(), QIcon::Normal, QIcon::Off);
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    if (repeat)
	{
        button->setAutoRepeat(true);
        button->setAutoRepeatDelay(300);
        button->setAutoRepeatInterval(150);
	}
    return button;
}

void QDashWindow::InitWidgets()
{
    clock = new QClockWidget(this);
    clock->setObjectName(QString::fromUtf8("clock"));
    clock->setGeometry(QRect(5, 5, 170, 170));
    speed = new QSpeedWidget(this);
    speed->setObjectName(QString::fromUtf8("speed"));
    speed->setGeometry(QRect(5, 185, 170, 170));
    altitude = new QAltitudeWidget(this);
    altitude->setObjectName(QString::fromUtf8("altitude"));
    altitude->setGeometry(QRect(525, 125, 110, 110));
    satview = new QSatViewWidget(this);
    satview->setObjectName(QString::fromUtf8("satview"));
    satview->setGeometry(QRect(525, 245, 110, 110));
    heading = new QHeadingWidget(this);
    heading->setObjectName(QString::fromUtf8("heading"));
    heading->setGeometry(QRect(170, 5, 350, 350));
    level = new QLevelWidget(this);
    level->setObjectName(QString::fromUtf8("level"));
    level->setGeometry(QRect(170, 5, 350, 350));

    group = new QGroupBox(this);
    group->setGeometry(QRect(120, 0, 400, 360));
    group->setMinimumSize(QSize(400, 360));
    group->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
		"    background: transparent;\n"
		"    border: 0px;\n"
		"        margin-top: 0px;\n"
		"    font-size: 18px;\n"
		"    font-weight: bold;\n"
		"    color: #ffffff;\n"
		"}\n"
		"QToolButton \n"
		"{\n"
		"    background-color: transparent;\n"
		"}\n"
		""));
    
    map = new QMapWidget(settings,group);
    map->setObjectName(QString::fromUtf8("map"));
    map->setGeometry(QRect(0, 0, 400, 360));

    //canvas = new QWidget(group);
    //canvas->setGeometry(QRect(0,0,400,360));
    buttons[0] = PlaceButton("flag.svg",group);
    buttons[1] = PlaceButton("route.svg",group);
    buttons[2] = PlaceButton("hiker.svg",group);
    buttons[3] = PlaceButton("zoom-in.svg",group,true);
    buttons[4] = PlaceButton("zoom-out.svg",group,true);
    buttons[5] = PlaceButton("options.svg",group);
    buttons[6] = PlaceButton("exit.svg",group);
    QMapStatusBar* statusbar = new QMapStatusBar(group);
    buttons[7] = statusbar;
    PositionButtons();
    group->show();
    
    gauges[0] = heading;
    gauges[1] = clock;
    gauges[2] = speed;
    gauges[3] = satview;
    gauges[4] = level;
    gauges[5] = altitude;

    mapper = new QSignalMapper(this);
    for (int i=0; i<6; i++)
    {
        connect(gauges[i], SIGNAL(singleTap()), mapper, SLOT(map()));
        mapper->setMapping(gauges[i], i+1);
    }
    connect(mapper,SIGNAL(mapped(const int &)),this,SLOT(ZoomToGauge(const int &)));

    connect(buttons[0],SIGNAL(clicked()),map,SLOT(ShowWaypointDialog()));
    connect(buttons[1],SIGNAL(clicked()),map,SLOT(ShowRouteDialog()));
    connect(buttons[2],SIGNAL(clicked()),map,SLOT(ShowTrackDialog()));
    connect(buttons[3],SIGNAL(clicked()),map,SLOT(zoomIn()));
    connect(buttons[4],SIGNAL(clicked()),map,SLOT(zoomOut()));
    connect(buttons[5],SIGNAL(clicked()),map,SLOT(ShowMenuDialog()));
    connect(buttons[6],SIGNAL(clicked()),this,SLOT(close()));
    connect(buttons[7],SIGNAL(clicked()),map,SLOT(ShowMapDialog()));
    
    connect(map,SIGNAL(name(QString)),statusbar,SLOT(SetTopLine(QString)));
    connect(map,SIGNAL(position(QString)),statusbar,SLOT(SetBottomLine(QString)));
    connect(map,SIGNAL(statuscolor(QPen)),statusbar,SLOT(SetPen(Pen)));
    map->SendMapInfo();
}

void QDashWindow::PositionButtons()
{
    int x,y,w,h;
    //w=group->width();
    //h=group->height();
    if (landscape)
    	if (mapzoomed)
    		{ w = 640; h = 360; }
    	else
    		{ w = 400; h = 360; }
    else
    	if (mapzoomed)
    		{ w = 360; h = 640; }
    	else
    		{ w = 360; h = 400; }
    	
    for (int i=0; i<8; i++)
	{
	   x = buttonpositions[i][0];
	   y = buttonpositions[i][1];
	   if (x<0) x+= w;
	   if (y<0) y+= h;
	   if (buttons[i])
       	   buttons[i]->move(x,y);
	}
}

void QDashWindow::ToggleMap()
{
    mapzoomed = !mapzoomed;
    settings.setValue("dash/mapzoomed",mapzoomed);
    Setup();
    PositionButtons();
    update();
}

void QDashWindow::Setup()
{
    if (mapzoomed)
    {
        for (int i =0; i<6; i++)
            gauges[i]->hide();

        group->setGeometry(0,0,width(),height());
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
            group->hide();
        else                        // Map visibile
        {
            if (landscape)          // Landscape
            {
                group->setGeometry(120,0,400,360);
                map->setGeometry(0,0,400,360);
            }
            else                    // Portrait
            {
                group->setGeometry(0,120,360,400);
                map->setGeometry(0,0,360,400);
            }
            group->show();
        }
    }
    else                            // In Transition
    {
        int delta = int(zoomstep/STEPCOUNTF * 360);
        if (tozoom == 0)
        {
            if (landscape)
            {
                group->setGeometry(120,0+delta,400,360+delta);
                map->setGeometry(0,0,400,360+delta);
            }
            else
            {
                group->setGeometry(0+delta,120,360+delta,400);
                map->setGeometry(0,0,360+delta,400);
            }
            group->show();
        }
        if (zoomgauge == 0)
        {
            if (landscape)
            {
                group->setGeometry(120,360-delta,400,720-delta);
                map->setGeometry(0,0,400,720-delta);
            }
            else
            {
                group->setGeometry(360-delta,120,720-delta,400);
                map->setGeometry(0,0,720-delta,400);
            }
            group->show();
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
        settings.setValue("dash/zoomgauge",zoomgauge);
        zoomtimer->stop();
        Setup();
        update();
    }
}

void QDashWindow::resizeEvent ( QResizeEvent * event )
{
    landscape = (width() > height());
    Setup();
    PositionButtons();
    settings.sync();
    QMainWindow::resizeEvent(event);
}

QDashWindow::~QDashWindow()
{
}
