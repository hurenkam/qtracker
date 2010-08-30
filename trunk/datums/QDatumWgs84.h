#ifndef QDATUMWGS84_H_
#define QDATUMWGS84_H_

#include <QStringList>
#include "qdatuminterface.h"

class DatumWgs84: public Datum
{
public:
	DatumWgs84() 
	: Datum("Wgs84", "Wgs84") )
	{ attributes.append("latitude"); attributes.append("longitude"); }
	
    virtual QStringList Representation(const WayPoint& w) const;
    virtual WayPoint Position(const QStringList& attr) const;
};

class DatumUTM: public Datum
{
public:
	DatumUTM() 
	: Datum("UTM", "UTM") )
	{ attributes.append("latitude"); attributes.append("longitude"); }
	
    virtual QStringList Representation(const WayPoint& w) const;
    virtual WayPoint Position(const QStringList& attr) const;
};

#endif /* QDATUMWGS84_H_ */
