#ifndef GpxIO_GpxIO_h
#define GpxIO_GpxIO_h

#include <QString>
#include <QList>
#include <QMap>
#include <QXmlStreamReader>
#include "geodata.h"

class XmlIO
{
protected:
    QXmlStreamReader xml;
    QXmlStreamReader::TokenType tt;

public:
    QString ReadElementString()            { return xml.readElementText(); }
    double  ReadElementDouble()            { return xml.readElementText().toDouble(); }
    QString ReadAttributeString(QString a) { return xml.attributes().value(a).toString(); }
    double  ReadAttributeDouble(QString a) { return xml.attributes().value(a).toString().toDouble(); }

};

class GpxIO: public XmlIO
{
public:
    static GpxIO* Instance() { if (!instance) instance = new GpxIO(); return instance; }
private:
    static GpxIO* instance;
	GpxIO()
    : wptlist(WayPointList::Instance())
	{ 
		//wptlist = WayPointList::Instance();
		rtelist = RouteList::Instance();
		trklist = TrackList::Instance();
		maplist = MapList::Instance();
	}

protected:
    WayPointList& wptlist;
    RouteList* rtelist;
    TrackList* trklist;
    MapList* maplist;

public:
    void ImportGpxFile(QString filename);
    
    MapMetaData* ReadMapMetaFile(QString filename);
    void WriteMapMetaFile(const MapMetaData& m);

private:
    void ReadGpx();
    WayPoint* ReadWayPoint(QString tag);
    Route* ReadRoute();
    Track* ReadTrack();
    AreaMetaData* ReadAreaMetaData(QString tag);
    MapMetaData* ReadMapMetaData(QString tag);
    Resolution ReadResolution();
    RefPoint ReadRefPoint();
    Bounds ReadBounds();
    void ReadExtensions();
};

#endif
