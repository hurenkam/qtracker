/*
 * datumlist.h
 *
 *  Created on: 31 aug 2010
 *      Author: Mark Hurenkamp
 */

#ifndef DATUMLIST_H_
#define DATUMLIST_H_

#include <QObject>
#include <QMap>
#include <QString>
#include <QSettings>
#include "waypointlist.h"
#include "projects.h"

class P4Pos
{
public:
	P4Pos();
	P4Pos(const WayPoint& w);
	P4Pos(double lat, double lon, double ele=0);
	
	double Latitude();
	double Longitude();
	double Elevation();
	
	double x,y,z;
};

class Parser
{
public:
    virtual QString fromPos    (P4Pos   p) = 0;
    virtual P4Pos   fromString (QString s) = 0;
};

class XYParser: public Parser
{
	virtual QString fromPos(P4Pos p)
    {
    	QString result;
    	result.sprintf("%7.0f %7.0f",p.x,p.y);
        return result;
    }
	virtual P4Pos fromString (QString s)
    {
		P4Pos result;
        QStringList list = s.split(" ", QString::SkipEmptyParts);
    	result.x = list[0].toDouble();
    	result.y = list[1].toDouble();
    	return result;
    }
};

class LatLonParser: public Parser
{
	virtual QString fromPos(P4Pos p)
    {
    	QString result;
    	result.sprintf("%8.5f %8.5f",p.Latitude(),p.Longitude());
        return result;
    }
	virtual P4Pos fromString (QString s)
    {
        QStringList list = s.split(" ", QString::SkipEmptyParts);
    	return P4Pos(list[0].toDouble(),list[1].toDouble());
    }
};

class Datum: public QObject
{
	Q_OBJECT
public:
	static Datum* wgs84;
	static Datum* utm;
	
	Datum(QString def, Parser* p);
	virtual ~Datum();
	virtual QString Forward(const WayPoint& p); 
	virtual WayPoint Inverse(const QString& p);
	
	virtual void SetAttribute(const QString&, const QString&);
	virtual QString GetAttribute(const QString&);
	virtual bool SupportsAttribute(const QString&);
	
protected:
	void SetProjection(QString proj4str);
	void ClearProjection();
	bool HasProjection();
	
private:
	static PJ* ref;
	PJ*        self;
    Parser*    parser;
    //QString    proj4str;
    QMap<QString,QString> attributes;
};

class UtmDatum: public Datum
{
// This could be a generic placeholder for many (most?) custom utm datums,
// provided it supports the 3 & 7 towgs parameters
// see http://svn.osgeo.org/metacrs/proj/trunk/proj/html/gen_parms.html
// and http://osgeo-org.1803224.n2.nabble.com/How-to-use-towgs84-td2393158.html

public:
	UtmDatum(Parser* p, int z=-1);
	void SetZone(int z, bool n=true);
	int SetZone(const QString& s);
	void SetEllipse(QString s);
	void SetToWgs(const QString& s);
	QString GetEllipse();
	QString GetToWgs();
	
    virtual QString Forward(const WayPoint& p);
	virtual WayPoint Inverse(const QString& p);
	virtual void SetAttribute(const QString& key, const QString& value); 
	virtual QString GetAttribute(const QString& key); 
	
protected:
	void FindZone(const WayPoint& p);
	void StripZone(QString& s);
	void SetProjection();
	
private:
	QSettings settings;
	int zone;
	bool north;
	QString ellipse;
	QString towgs;
};

class DatumList: public QObject
{
	Q_OBJECT
public:
    static DatumList& Instance();
    DatumList();
    virtual ~DatumList();
    
    void AddDatum(QString name, Datum* d);
    QString Representation(const WayPoint& w);
    WayPoint Position(const QString& r);
    QStringList Keys();
    QStringList Ellipses();
    void SetDatum(QString datum);
    QString GetDatum();

public slots:
    void ReadSettings();
	void SaveSettings();
	
private:
    static DatumList* instance;
    QSettings settings;
    QString datum;
	XYParser xy;
	LatLonParser latlon;
    QMap<QString, Datum*> datums;
    QStringList ellipses;
};

#endif /* DATUMLIST_H_ */
