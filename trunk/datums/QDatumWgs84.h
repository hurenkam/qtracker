/*
 * QDatumWgs84.h
 *
 *  Created on: 30-okt-2009
 *      Author: nly99036
 */

#ifndef QDATUMWGS84_H_
#define QDATUMWGS84_H_

#include <QStringList>
#include "qdatuminterface.h"

class QWgs84DisplayWidget: public QDatumDisplayWidget
{
	Q_OBJECT
public:
	QWgs84DisplayWidget(QWidget *parent);
	virtual void setValues(double lat, double lon, int src=0)
		{ latitude = lat; longitude = lon; source = src; update(); }
	
protected:
    virtual void paintEvent(QPaintEvent *event);
    
private:
	double latitude;
	double longitude;
	int source;
};

class QWgs84EditWidget: public QDatumEditWidget
{
	Q_OBJECT
public:
	QWgs84EditWidget(QWidget *parent);
	virtual void setValues(double lat, double lon, int src=0)
		{ latitude = lat; longitude = lon; source = src; update(); }
	virtual void values(double& lat, double& lon, int& src)
		{ lat = latitude; lon = longitude; src = source; }
private:
	double latitude;
	double longitude;
	int source;
};


class QDatumWgs84
{
public:
	QDatumWgs84() {};
	virtual ~QDatumWgs84() {};

	virtual QStringList DatumList()
	{ QStringList list; list.append(QString("Wgs84")); return list; }
	virtual QDatumDisplayWidget *DisplayWidget(QString datum, QWidget *parent=0, int mode=0)
	{ return new QWgs84DisplayWidget(parent); }
	virtual QDatumEditWidget *EditWidget(QString datum, QWidget *parent=0, int mode=0)
	{ return new QWgs84EditWidget(parent); }
};

#endif /* QDATUMWGS84_H_ */
