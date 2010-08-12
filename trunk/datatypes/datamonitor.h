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
#include <QGeoSatelliteInfo>
#include <QGeoPositionInfo>
#include <QCompassReading>
#include <QSystemNetworkInfo>
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
    class QGeoPositionInfoSource;
    class QGeoSatelliteInfoSource;
    class QCompass;
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
	void TimeUpdated(long);

public:
	MonitorStrategy(int t=0) : type(t) {}
	virtual void OnPositionUpdate(const QGeoPositionInfo& info) {}
	virtual void OnTimeUpdate(const QTime& time) {}
	virtual void OnHeadingUpdate(int heading) {}
	int Type() { return type; }
	
protected:
	void SetType(int t) { type = t; }
private:
	int type;
};

class WayPointStrategy: public MonitorStrategy
{
public:
	WayPointStrategy(const WayPoint& wpt);
	WayPointStrategy(const QString& wpt);
	virtual void OnTimeUpdate(const QTime& time);
	
	virtual void OnPositionUpdate(const QGeoPositionInfo& info);
private:
	QString name;
	QGeoPositionInfo currentinfo;
	QGeoCoordinate currentposition;
	QGeoCoordinate targetposition;
};

class Filter: public QObject
{
	Q_OBJECT
signals:
	void ValueUpdated(double);
	
public slots:
	virtual void NewValue(double v) {}
	virtual bool IsValid() { return true; }
};

class AverageFilter: public Filter
{
protected:
    int length;
    QList<double> values;

public:
	AverageFilter(int l) { SetLength(l); }
    virtual double Value() 
    { 
    	if (values.length()<=0) return 0;
    	
        double r=0; 
        for (int i=0; i<values.length(); i++) 
        	r += values[i]; 
        return r/values.length(); 
    }
	
    virtual void NewValue(double v)
    {
        values.append(v);
        
        if (values.length() > length)
        	values.removeFirst();
        
        emit ValueUpdated(Value());
    }
    
    void SetLength(int l)
    {
    	if (l > 0)
            length = l;
    	else
    		length = 1;
    	
    	while (length < values.length())
    		values.removeFirst();
    }
    
    virtual bool IsValid()
	{
        return (values.length()>0);
	}
};

class HeadingFilter: public AverageFilter
{
public:
	HeadingFilter(int l) : AverageFilter(l) {}
    virtual double Value() 
    { 
    	if (values.length()<=0) return 0;
    	if (values.length()==1) return values[0];
    	
        double start = values[0];
        QList<double> deltas;
        
        for (int i=1; i<values.length(); i++)
        {
            double delta = values[i] - start;
            if (delta < -180) delta += 360;
            if (delta > 180)  delta -= 360;
            deltas.append(delta);
        }
        double avg = 0.0;
        for (int j=0; j<deltas.length(); j++)
            avg += deltas[j];
        
        avg = avg/deltas.length();
        return fmod((start + avg),360);
    }
};

class DataMonitor: public QObject
{
	Q_OBJECT
	
signals:
	void PositionUpdated(const QGeoPositionInfo&);
	void SatellitesInViewUpdated(const QList<QGeoSatelliteInfo>&);
	void SatellitesInUseUpdated(const QList<QGeoSatelliteInfo>&);
	
	void HeadingUpdated(double);
	void SpeedUpdated(double);
	void AltitudeUpdated(double);
	void HorizontalUpdated(double);
	void VerticalUpdated(double);
	
	void BearingUpdated(double);
	void DistanceUpdated(double);
	void TimeUpdated(long seconds);
	
public slots:
    void OnPositionUpdate(const QGeoPositionInfo& info);
	void OnHeadingUpdate();
	void OnTimeUpdate();
	void SetStrategy(MonitorStrategy *s);
	void UpdateSatsInUse(const QList<QGeoSatelliteInfo>& inuse)   { satsinuse = inuse;   emit SatellitesInUseUpdated(inuse); } 
	void UpdateSatsInView(const QList<QGeoSatelliteInfo>& inview) { satsinview = inview; emit SatellitesInViewUpdated(inview); } 
	
public:
    static DataMonitor& Instance();
	virtual ~DataMonitor();
	int  HeadingSource()           { settings.sync(); return settings.value("compass/source",0).toInt(); }
	double CompassCalibration()    { if (HasCompassSensor()) return reading->calibrationLevel(); else return 0; }
	bool HasCompassSensor()        { return ((compass != 0) && (reading != 0)); }
	bool IsUsingGPSCompass()       { return ((HeadingSource()==1) || (!HasCompassSensor())); }
	const QList<QGeoSatelliteInfo>& SatsInUse() { return satsinuse; }
	const QList<QGeoSatelliteInfo>& SatsInView() { return satsinview; }
	
	double Speed()                 { return speed->Value();    }
	double Heading()               { return heading->Value();  }
	double Altitude()              { return altitude->Value(); }
	double Horizontal()            { if (horizontal->IsValid()) return horizontal->Value(); else return 10000; }
	double Vertical()              { if (vertical->IsValid())   return vertical->Value();   else return 10000; }
	
private:
    static DataMonitor* instance;
	DataMonitor();
	
private:
	MonitorStrategy* strategy;
    QSettings settings;
	QGeoPositionInfoSource* possource;
    QGeoSatelliteInfoSource* satsource;
    QList<QGeoSatelliteInfo> satsinuse;
    QList<QGeoSatelliteInfo> satsinview;
    QCompass* compass;
    QCompassReading* reading;

    HeadingFilter* heading;
    AverageFilter* speed;
    AverageFilter* altitude;
    AverageFilter* horizontal;
    AverageFilter* vertical;
}; 

#endif /* DATAMONITOR_H_ */
