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
    void updateSpeed(double speed);
    void updateHeading(double course);
    void updateSatInfo(int id, int strength, double azimuth, double elevation, bool inuse);
    
protected:
	virtual void resizeEvent(QResizeEvent * event);
	void Setup640x360();
	void Setup360x640();
private:
    QClockWidget *clock;
    QSpeedWidget *speed;
    QClockWidget *timer;
    QAltitudeWidget *altitude;
    QSatViewWidget *satview;
    QHeadingWidget *heading;
    
    XQLocation location;
    int counter;
    QTime starttime;
    
    float altmin;
    float altmax;
    bool  altvalid;
    
    int   count;
    float althist[historysize];
    float speedhist[historysize];
    float headhist[historysize];
    
    double prevlat;
    double prevlon;
    double distance;
    
    bool timevalid;
    bool posvalid;
};

#endif /* QDASHWINDOW_H_ */
