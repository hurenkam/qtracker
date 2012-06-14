#ifndef GpxIO_GpxIO_h
#define GpxIO_GpxIO_h

#include <QString>
#include <QList>
#include <QMap>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "geodata.h"

class WayPointList;
class RouteList;
class TrackList;
class MapList;
class WayPoint;
class RefPoint;
class Resolution;
class Route;
class Track;
class MapMetaData;

class XmlIO
{
protected:
    QXmlStreamReader reader;
    QXmlStreamWriter writer;
    QXmlStreamReader::TokenType tt;

public:
    QString ReadElementString()            { return reader.readElementText(); }
    double  ReadElementDouble()            { return reader.readElementText().toDouble(); }
    QString ReadAttributeString(QString a) { return reader.attributes().value(a).toString(); }
    double  ReadAttributeDouble(QString a) { return reader.attributes().value(a).toString().toDouble(); }

};

class GpxIO: public XmlIO
{
public:
    static GpxIO* Instance() { if (!instance) instance = new GpxIO(); return instance; }
private:
    static GpxIO* instance;
	GpxIO();

protected:
    WayPointList& wptlist;
    RouteList& rtelist;
    TrackList& trklist;
    MapList& maplist;

public:
    void ImportGpxFile(QString filename);
    MapMetaData* ReadMapMetaFile(QString filename);
    
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

public:    
    void WriteMapMetaFile(const MapMetaData& m);
    void WriteTrackFile(const Track& t);
    void WriteRouteFile(const Route& r);

private:
    void WriteGpxHeader();
    void WriteGpxFooter();
    void WriteTrack(const Track& t);
    void WriteRoute(const Route& r);
    void WriteWayPoint(const WayPoint& w, QString tag);
    void WriteMapMetaData(const MapMetaData& m, QString tag);
    void WriteResolution(const Resolution& r);
    void WriteRefPoint(const RefPoint& r);
};

#endif
