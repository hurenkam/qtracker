/*
 * datamonitor.h
 *
 *  Created on: 30 jul 2010
 *      Author: Mark Hurenkamp
 */

#ifndef DATAMONITOR_H_
#define DATAMONITOR_H_

#include <QObject>
#include <QWidget>
#include <QSettings>
#include <QGeoCoordinate>
class QDateTime;
class QSettings;
class WayPoint;
class QGroupBox;
class QButtonGroup;
class QListWidget;
class QComboBox;
class QWidget;
class QRadioButton;
class QHeadingWidget;
class QBoxLayout;


namespace QtMobility
{
    class QGeoPositionInfo;
    class QGeoPositionInfoSource;
    class QCompass;
    class QCompassReading;
}
using namespace QtMobility;

class QMonitorOptions: public QWidget
{
	Q_OBJECT

signals:
	void changed();
	
public:
    QMonitorOptions(QDialog* d);
    ~QMonitorOptions();
    
protected:
    virtual void resizeEvent(QResizeEvent * event);

public slots:
    void noneselected();
    void waypointselected();
    void routeselected();
    void trackselected();
    
    void apply();
    
private:
    QSettings settings;
	QBoxLayout* center;
    QGroupBox* typegroup;
    QButtonGroup* typebuttons;
    QListWidget* wptcombo;
    QListWidget* trkcombo;
    QListWidget* rtecombo;
    QWidget* filler;
    QStringList list;
    QString wptname;
    QString trkname;
    QString rtename;
    int montype;
};

class MonitorStrategy: public QObject
{
	Q_OBJECT

signals:
	void BearingUpdated(double);
	void DistanceUpdated(double);
	void TimeUpdated(const QDateTime&);

public:
	virtual void OnPositionUpdate(const QGeoPositionInfo& info) {}
	virtual void OnTimeUpdate(const QDateTime& time) {}
	virtual void OnHeadingUpdate(int heading) {}
};

class WayPointStrategy: public MonitorStrategy
{
public:
	WayPointStrategy(const WayPoint& wpt);
	WayPointStrategy(const QString& wpt);
	
	virtual void OnPositionUpdate(const QGeoPositionInfo& info);
private:
	QString name;
	QGeoCoordinate currentposition;
	QGeoCoordinate targetposition;
};

class DataMonitor: public QObject
{
	Q_OBJECT
	
signals:
	void PositionUpdated(const QGeoPositionInfo&);
	void HeadingUpdated(double);
	
	void BearingUpdated(double);
	void DistanceUpdated(double);
	void TimeUpdated(const QDateTime&);
	
public slots:
    void OnPositionUpdate(const QGeoPositionInfo& info);
	void OnHeadingUpdate();
	void OnTimeUpdate();
	void SetStrategy(MonitorStrategy *s);
	
public:
    static DataMonitor& Instance();
	virtual ~DataMonitor();
	
private:
    static DataMonitor* instance;
	DataMonitor();
	
private:
    QSettings* settings;
	QGeoPositionInfoSource* possource;
    QCompass *compass;
    QCompassReading* reading;
    MonitorStrategy* strategy;
};

#endif /* DATAMONITOR_H_ */
