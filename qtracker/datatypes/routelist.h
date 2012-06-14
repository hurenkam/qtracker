/*
 * geodata.h
 *
 *  Created on: 8-jul-2010
 *      Author: Mark Hurenkamp
 */

#ifndef ROUTELIST_H_
#define ROUTELIST_H_

#include <QString>
#include <QList>
#include <QMap>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include "ui.h"
#include "geodata.h"

class Route: public QObject
{
    Q_OBJECT
signals:
    void updated(WayPoint& w);
protected:
	QString name;
	QList<WayPoint*> list;
public:
	QString Name() const             { return name; }
	QString FileName() const         { return QString(GetDrive() + QString(ROUTEDIR) + name + ".gpx"); }
	void SetName(QString n)          { name = n; }
	void AddPoint(WayPoint* w)       { list.append(w); emit updated(*w); }
	WayPoint& GetItem(int i) const   { return *list[i]; }
	int Length() const               { return list.length(); }
};

class RouteList: public QObject
{
    Q_OBJECT
    
signals:
	void added(const QString&);
	void updated(const QString&, const QString&);
	void deleted(const QString&);
	void visible(const QString&);
	void invisible(const QString&);

public:
    static RouteList* Instance() { if (!instance) instance = new RouteList(); return instance; }

private:
    static RouteList* instance;
    RouteList();
    ~RouteList();
    QStringList FindFiles();

protected:
	QMap<QString, Route*> map;
    QSettings settings;
	QStringList routefiles;
	
public slots:
	void SaveSettings();
	void Hide(const QString& key);
	void Show(const QString& key);
	void Delete(const QString& key);
	//void AddRoute(const Route& t)          { AddRoute(new Route(t); }
	//void UpdateRoute(const QString& orgname, const Route& t) {}
	
public:
	void AddRoute(Route* t);
	void AddMetaData(AreaMetaData* m);
	QStringList Keys();
	QStringList VisibleKeys();
	QStringList HiddenKeys();
	QStringList AreaKeys(Bounds a);
	QStringList VisibleAreaKeys(Bounds a);
	bool IsVisible(const QString& k);
	Route& GetItem(const QString& k) const;
};


#endif /* ROUTELIST_H_ */
