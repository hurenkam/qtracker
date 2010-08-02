/*
 * QDashWindow.h
 *
 *  Created on: 4-okt-2009
 *      Author: Mark
 */

#ifndef QDASHWINDOW_H_
#define QDASHWINDOW_H_

#include <QtGui>
#include <QList>
#include <QCompass>
#include <QCompassReading>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfo>
#include <QGeoSatelliteInfoSource>
class QClockWidget;
class QSpeedWidget;
class QAltitudeWidget;
class QSatViewWidget;
class QHeadingWidget;
class QMapWidget;
class QSplashScreen;

using namespace QtMobility;

const int historysize = 5;

class QDashWindow: public QMainWindow
{
	Q_OBJECT

public:
	QDashWindow(QSettings& s, QWidget *parent=0);
	virtual ~QDashWindow();
	void Init(QSplashScreen *s=0);

public slots:
    void timeChanged();
    void ZoomTimerExpired();
    void ZoomToGauge(int i);
    void resetTimer();
    void ToggleMap();

protected:
    void InitWidgets();
    void StartTransition(int to);

    virtual void resizeEvent(QResizeEvent * event);
    void Setup();

private:
    QSettings& settings;
    QClockWidget *clock;
    QSpeedWidget *speed;
    QClockWidget *timer;
    QAltitudeWidget *altitude;
    QSatViewWidget *satview;
    QHeadingWidget *heading;
    QMapWidget *map;
    QSignalMapper *mapper;
    QWidget* gauges[6];

    QSensor *compass;
    QCompassReading *reading;
    QTimer *zoomtimer;

    int zoomstep;
    int zoomgauge;
    int tozoom;
    bool landscape;
    QTime starttime;
    int   count;
    bool timevalid;
    bool mapzoomed;
};

#endif /* QDASHWINDOW_H_ */
