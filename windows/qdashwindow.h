/*
 * QDashWindow.h
 *
 *  Created on: 4-okt-2009
 *      Author: Mark
 */

#ifndef QDASHWINDOW_H_
#define QDASHWINDOW_H_

#include <QtGui>
#include "xqlocation.h"
class QClockWidget;
class QSpeedWidget;
class QAltitudeWidget;
class QSatViewWidget;
class QHeadingWidget;
class QMapWidget;

const int historysize = 5;

class QDashWindow: public QMainWindow
{
        Q_OBJECT

public:
        QDashWindow(QWidget *parent=0);
        virtual ~QDashWindow();

public slots:
    void timeChanged();
    void locationChanged(double latitude, double longitude, double altitude, float speed, float heading);
    void updateDistance(double lat,double lon);
    void updateAltitude(double altitude);
    void updateSpeed(float speed);
    void updateHeading(float course);
    void updateSatInfo(int id, int strength, double azimuth, double elevation, bool inuse);
    void ZoomTimerExpired();
    void ZoomToGauge(int i);
    void GaugeOptions(int i);

protected:
    void InitWidgets();
    void StartTransition(int to);

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

    XQLocation location;
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
};

#endif /* QDASHWINDOW_H_ */
