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
	QDashWindow(QWidget *parent=0);
	virtual ~QDashWindow();
	void Init(QSplashScreen *s=0);

public slots:
    void timeChanged();
    //void updateHeading();
    void updatePosition(const QGeoPositionInfo &info);
    void updateSatellitesInView(const QList<QGeoSatelliteInfo> &info);
    void updateSatellitesInUse(const QList<QGeoSatelliteInfo> &info);
    void ZoomTimerExpired();
    void ZoomToGauge(int i);
    void GaugeOptions(int i);
    void resetDistance();
    void resetTimer();
    void ToggleMap();
    void clearSatellites();

protected:
    void InitWidgets();
    void StartTransition(int to);
    void updateSatelliteList();
    void updateDistance(double lat,double lon);

    virtual void resizeEvent(QResizeEvent * event);
    void Setup();

private:
    QClockWidget *clock;
    QSpeedWidget *speed;
    QClockWidget *timer;
    QAltitudeWidget *altitude;
    QSatViewWidget *satview;
    QHeadingWidget *heading;
    QMapWidget *map;
    QSignalMapper *mapper;
    QWidget* gauges[6];
    QGeoPositionInfoSource *possource;
    QGeoSatelliteInfoSource *satsource;
    QList<QGeoSatelliteInfo> inUse;
    QList<QGeoSatelliteInfo> inView;

    //QCompass *compass;
    QTimer *zoomtimer;

    int zoomstep;
    int zoomgauge;
    int tozoom;
    bool landscape;

    QTime starttime;

    int   count;

    double prevlat;
    double prevlon;
    double distance;

    bool timevalid;
    bool posvalid;

    bool showmap;
    bool mapzoomed;
};

#endif /* QDASHWINDOW_H_ */
