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
	const QString Name() const        { return name; }
	const QString FileName() const    { return QString(GetDrive() + QString(TRACKDIR) + name + ".gpx"); }
	void SetName(QString n)           { name = n; }
	void AddPoint(WayPoint* w)        { list.append(w); emit updated(*w); }
};

class RouteList: public QObject
{
    Q_OBJECT
signals:
    void updated(QString n);
public:
    static RouteList* Instance() { if (!instance) instance = new RouteList(); return instance; }
private:
    static RouteList* instance;
    RouteList() {};

protected:
	QMap<QString, Route*> map;
	
public:
	// Todo: handle case if name already exists
	void AddRoute(Route* r)            { map[r->Name()]=r; emit updated(r->Name()); }
	void AddMetaData(AreaMetaData* m)  { }
};

#endif /* ROUTELIST_H_ */
