/*
 * geodata.h
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#ifndef WAYPOINTLIST_H_
#define WAYPOINTLIST_H_

#include <QString>
#include <QList>
#include <QMap>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include "ui.h"
#include "geodata.h"

class WayPoint
{
protected:
	QString name;
	double latitude;
	double longitude;
	double elevation;
	QString time;
public:
	WayPoint(QString s, QString n="");
	
	WayPoint(double lat=0, double lon=0, double ele=0.0, QString t="", QString n="")
        : name(n), latitude(lat), longitude(lon), elevation(ele), time(t) {}
	
	WayPoint(const WayPoint& w)
        : name(w.Name())
        , latitude(w.Latitude())
        , longitude(w.Longitude())
        , elevation(w.Elevation())
        , time(w.Time())
        {}

	QString Name() const               { return name; }
	double Latitude() const            { return latitude; }
	double Longitude() const           { return longitude; }
	double Elevation() const           { return elevation; }
	QString Time() const               { return time; }
	
	void SetName(QString n)            { name = n; }
	void SetLatitude(double l)         { latitude = l; }
	void SetLongitude(double l)        { longitude = l; }
	void SetElevation(double e)        { elevation = e; }
	void SetTime(QString t)            { time = t; }
	QString Representation(geodata::Datum datum) const;

	double distance(WayPoint *to);
	QString distancestr(WayPoint *to);
	double bearing(WayPoint *to);
};

class WayPointList : public QObject
{
    Q_OBJECT
signals:
    void added(const QString&);
    void updated(const QString&, const QString&);
    void removed(const QString&);
    void visible(const QString&);
    void invisible(const QString&);
    
public:
    static WayPointList& Instance();
private:
    static WayPointList* instance;
    WayPointList();
    ~WayPointList();
protected:
    QSettings settings;
    QStringList visiblekeys;
	QMap<QString, WayPoint*> map;

public slots:
	void SaveSettings();
	void AddWayPoint(const WayPoint& w);
	void RemoveWayPoint(const QString& s);
	void Hide(const QString& key);
	void Show(const QString& key);
	
public:
	void AddWayPoint(WayPoint* w,bool visible=true);
	void UpdateWayPoint(const QString& orgname, const WayPoint& w);
	QStringList Keys();
	QStringList VisibleKeys();
	QStringList HiddenKeys();
	QStringList AreaKeys(Bounds a);
	QStringList VisibleAreaKeys(Bounds a);
	bool IsVisible(const QString& k);
	const WayPoint& GetItem(const QString& n);
	QString FileName();
};

#endif /* WAYPOINTLIST_H_ */
