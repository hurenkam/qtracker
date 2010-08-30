#ifndef QDATUMINTERFACE_H_
#define QDATUMINTERFACE_H_

#include <QStringList>
#include "waypointlist.h"

class Datum {
public:
	void Datum(const QString& ref, const QString& desc) 
    : name(ref), description(desc), attributes(attr)
    { attributes.clear(); }
	QString Name() const           { return name; }
	QString Description() const    { return description; }
	QStringList Attributes() const { return attributes; } 

    virtual QStringList Representation(const WayPoint& w) const = 0;
    virtual WayPoint Position(const QStringList& attr) const = 0;
	
protected:
	QString name;
	QString description;
	QStringList attributes;
};

class DatumInterface
{
public:
    DatumInterface();
    virtual ~DatumInterface();
	
    void RegisterDatum(const Datum& d) 
    { list[d.Name()] = d; }
	
    QStringList GetRepresentation(const QString& ref, const WayPoint& w)
    { return list[ref].Representation(w); } 
	
    WayPoint GetPosition(const QString& ref, const QStringList& attr)
    { return list[ref].Position(attr); }
	
private:
	QMap<QString,Datum> list;
};
#endif /* QDATUMINTERFACE_H_ */
