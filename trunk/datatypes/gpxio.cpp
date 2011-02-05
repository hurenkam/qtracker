#include <QFile>
#include <QXmlStreamReader>
#include <QString>
#include "gpxio.h"
#include "waypointlist.h"
#include "routelist.h"
#include "tracklist.h"
#include "maplist.h"

#include <QDebug>
#include <iostream>
#define LOG( a ) qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 

//========================================================================

GpxIO* GpxIO::instance = 0;

GpxIO::GpxIO()
: wptlist(WayPointList::Instance())
, rtelist(*RouteList::Instance())
, trklist(*TrackList::Instance())
, maplist(MapList::Instance())
{ 
}

void GpxIO::ImportGpxFile(QString filename)
{
    LOG( "GpxIO::ImportGpxFile(" << filename; )
    
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	reader.setDevice(&file);

	while (!reader.atEnd())
	{
        LOG2( "GpxIO::ImportGpxFile() xml.name(): " << reader.name(); )
    
		if (reader.name()=="gpx")
			ReadGpx();
        
        reader.readNextStartElement();
	}
}

MapMetaData* GpxIO::ReadMapMetaFile(QString filename)
{
    LOG( "GpxIO::ReadMapMetaFile(" << filename; )

	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	reader.setDevice(&file);
	
    return ReadMapMetaData("map");
}

//========================================================================

void GpxIO::ReadGpx()
{
    LOG( "GpxIO::ReadGpx()"; )

	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "gpx"))
	{
        LOG2( "GpxIO::ReadGpx() xml.name(): " << reader.name(); )
    
        if (reader.tokenType()==QXmlStreamReader::StartElement)
        {
			//if (xml.name()=="time")       ReadElementString();
			//if (xml.name()=="bounds")     ReadBounds();
			if (reader.name()=="wpt")        wptlist.AddWayPoint(ReadWayPoint("wpt"));
			if (reader.name()=="rte")        rtelist.AddRoute(ReadRoute());
			if (reader.name()=="trk")        trklist.AddTrack(ReadTrack());
			if (reader.name()=="extensions") ReadExtensions();
        }
        reader.readNext();
	}
}

WayPoint* GpxIO::ReadWayPoint(QString tag="wpt")
{
    LOG2( "GpxIO::ReadWayPoint()"; )
    
	WayPoint* wpt = new WayPoint();
	wpt->SetLatitude(ReadAttributeDouble("lat"));
	wpt->SetLongitude(ReadAttributeDouble("lon"));
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == tag))
	{
        LOG3( "GpxIO::ReadWayPoint() readername(): " << reader.name(); )
    
		if (reader.tokenType()==QXmlStreamReader::StartElement)
		{
			if (reader.name()=="ele")  wpt->SetElevation(ReadElementDouble());
			if (reader.name()=="time") wpt->SetTime(ReadElementString());
			if (reader.name()=="name") wpt->SetName(ReadElementString());
			//if (reader.name()=="desc") ;
			//if (reader.name()=="sym") ;
			//if (reader.name()=="type") ;
		}
		reader.readNext();
	}
    LOG2( "GpxIO::ReadWayPoint() " << tag << ": " << wpt->Name() << ", " << wpt->Latitude() << ", " << wpt->Longitude(); )
	return wpt;
}

Route* GpxIO::ReadRoute()
{
    LOG( "GpxIO::ReadRoute()"; )

	Route* rte = new Route();
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "rte"))
	{
        LOG2( "GpxIO::ReadRoute() reader.name(): " << reader.name(); )
    
        
		if (reader.tokenType()==QXmlStreamReader::StartElement)
		{
			if (reader.name()=="name")  rte->SetName(ReadElementString());
			if (reader.name()=="rtept") rte->AddPoint(ReadWayPoint("rtept"));
		}
		
		reader.readNext();
	}
	return rte;
}

Track* GpxIO::ReadTrack()
{
    LOG( "GpxIO::ReadTrack()"; )

	Track* trk = new Track();
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "trk"))
	{
        LOG2( "GpxIO::ReadTrack() reader.name(): " << reader.name(); )
    
		if (reader.tokenType()==QXmlStreamReader::StartElement)
		{
			if (reader.name()=="name")  trk->SetName(ReadElementString());
			if (reader.name()=="trkpt") trk->AddPoint(ReadWayPoint("trkpt"));
		}
        reader.readNext();
	}
	return trk;
}

void GpxIO::ReadExtensions()
{
    LOG( "GpxIO::ReadExtensions()"; )

    while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "extensions"))
	{
        LOG2( "GpxIO::ReadExtensions() reader.name(): " << reader.name(); )
        
		if (reader.tokenType()==QXmlStreamReader::StartElement)
		{
			if (reader.name()=="mapmeta") maplist.AddMetaData(ReadMapMetaData("mapmeta"));
			if (reader.name()=="rtemeta") rtelist.AddMetaData(ReadAreaMetaData("rtemeta"));
			if (reader.name()=="trkmeta") trklist.AddMetaData(ReadAreaMetaData("trkmeta"));
		}
        reader.readNext();
	}
}

//========================================================================

AreaMetaData* GpxIO::ReadAreaMetaData(QString tag)
{
    LOG( "GpxIO::ReadAreaMetaData()"; )
	AreaMetaData* meta = new AreaMetaData();
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == tag))
	{
        LOG2( "GpxIO::ReadAreaMetaData() reader.name():" << reader.name(); )
		if (reader.tokenType()==QXmlStreamReader::StartElement)
		{
			if (reader.name()=="name")    meta->SetName(ReadElementString());
			if (reader.name()=="bounds")  meta->SetBounds(ReadBounds());
		}
		reader.readNext();
	}
	return meta;
}

MapMetaData* GpxIO::ReadMapMetaData(QString tag)
{
    LOG( "GpxIO::ReadMapMetaData()"; )
	MapMetaData* meta = new MapMetaData();
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == tag))
	{
        LOG2( "GpxIO::ReadMapMetaData() reader.name():" << reader.name(); )
		if (reader.tokenType()==QXmlStreamReader::StartElement)
		{
			if (reader.name()=="resolution") meta->SetResolution(ReadResolution());
			if (reader.name()=="refpoint")   meta->AddRefPoint(ReadRefPoint());
			if (reader.name()=="name")       meta->SetName(ReadElementString());
		}
		reader.readNext();
	}
	return meta;
}

Resolution GpxIO::ReadResolution()
{
    LOG( "GpxIO::ReadResolution()"; )
	Resolution r = Resolution(
			ReadAttributeDouble("width"),
			ReadAttributeDouble("height")
		);
    LOG( "GpxIO::ReadResolution() result: " << r.Width() << ", " << r.Height(); )
    return r;
}

RefPoint GpxIO::ReadRefPoint()
{
    LOG( "GpxIO::ReadRefPoint()"; )
	RefPoint r = RefPoint(
			ReadAttributeDouble("x"),
			ReadAttributeDouble("y"),
			ReadAttributeDouble("lat"),
			ReadAttributeDouble("lon")
		);
    LOG( "GpxIO::ReadRefPoint() result: " << r.X() << ", " << r.Y() << ", " << r.Latitude() << ", " << r.Longitude(); )
    return r;
}

Bounds GpxIO::ReadBounds()
{
    LOG( "GpxIO::ReadBounds()"; )
	Bounds b = Bounds(
			ReadAttributeDouble("minlat"),
			ReadAttributeDouble("minlon"),
			ReadAttributeDouble("maxlat"),
			ReadAttributeDouble("maxlon")
		);
    LOG( "GpxIO::ReadBounds() result: " << b.MinLatitude() << ", " << b.MinLongitude() << ", " << b.MaxLatitude() << ", " << b.MaxLongitude(); )
    return b;
}

//========================================================================

void GpxIO::WriteMapMetaFile(const MapMetaData& m)
{
    LOG( "GpxIO::WriteMapMetaFile(" << m.GetMetaFilename(); )

	QFile file(m.GetMetaFilename());
	file.open(QIODevice::ReadWrite);
	writer.setDevice(&file);
	writer.setAutoFormatting(true);
	WriteGpxHeader();
	WriteMapMetaData(m,"map");
	WriteGpxFooter();
	file.close();
}

void GpxIO::WriteTrackFile(const Track& t)
{
    LOG( "GpxIO::WriteTrackFile(" << t.FileName(); )

	QFile file(t.FileName());
	file.open(QIODevice::ReadWrite);
	writer.setDevice(&file);
	writer.setAutoFormatting(true);
	WriteGpxHeader();
	WriteTrack(t);
	WriteGpxFooter();
	file.close();
}

void GpxIO::WriteRouteFile(const Route& r)
{
    LOG( "GpxIO::WriteRouteFile(" << r.FileName(); )

	QFile file(r.FileName());
	file.open(QIODevice::ReadWrite);
	writer.setDevice(&file);
	writer.setAutoFormatting(true);
	WriteGpxHeader();
	WriteRoute(r);
	WriteGpxFooter();
	file.close();
}

//========================================================================

void GpxIO::WriteGpxHeader()
{	
    LOG( "GpxIO::WriteGpxHeader()"; )
	writer.writeStartDocument();
	writer.writeStartElement("gpx");
	writer.writeAttribute("version","1.0");
        writer.writeAttribute("creator","qTracker 0.8.270 http://qtracker.googlecode.com");
	writer.writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
	writer.writeAttribute("xmlns","http://www.topografix.com/GPX/1/0");
	writer.writeAttribute("xsi:schemaLocation","http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd");
}

void GpxIO::WriteGpxFooter()
{
    LOG( "GpxIO::WriteGpxFooter()"; )
	writer.writeEndElement();
	writer.writeEndDocument();
}

void GpxIO::WriteTrack(const Track& t)
{
    LOG( "GpxIO::WriteTrack()"; )
	writer.writeStartElement("trk");
	if (t.Name() != "" )
		writer.writeTextElement("name",t.Name());
	writer.writeStartElement("trkseg");
	for (int i=0; i<t.Length(); i++)
		WriteWayPoint(t.GetItem(i),"trkpt");
	writer.writeEndElement();
	writer.writeEndElement();
}

void GpxIO::WriteRoute(const Route& r)
{
    LOG( "GpxIO::WriteRoute()"; )
	writer.writeStartElement("rte");
	writer.writeEndElement();
}

void GpxIO::WriteWayPoint(const WayPoint& w, QString tag)
{
    LOG2( "GpxIO::WriteWayPoint()"; )
	writer.writeStartElement(tag);
	writer.writeAttribute("lat",QString::number(w.Latitude()));
	writer.writeAttribute("lon",QString::number(w.Longitude()));
	if (w.Name() != "" )
		writer.writeTextElement("name",w.Name());
	if (w.Time() != "" )
		writer.writeTextElement("time",w.Time());
	if (w.Elevation() != 0.0)
		writer.writeTextElement("ele",QString::number(w.Elevation()));
	writer.writeEndElement();
}

void GpxIO::WriteMapMetaData(const MapMetaData& m, QString tag)
{
    LOG( "GpxIO::WriteMapMetaData()"; )
	writer.writeStartElement(tag);
    writer.writeAttribute("imagefile",m.GetImageFilename());
	writer.writeTextElement("name",m.Name());
    WriteResolution(m.GetResolution());
    for (int i=0; i<m.Length(); i++)
    	WriteRefPoint(m.GetRefPoint(i));
    writer.writeEndElement();
}

void GpxIO::WriteResolution(const Resolution& r)
{
    writer.writeStartElement("resolution");
    writer.writeAttribute("width",QString::number(r.Width()));
    writer.writeAttribute("height",QString::number(r.Height()));
    writer.writeEndElement();
}

void GpxIO::WriteRefPoint(const RefPoint& r)
{
    LOG2( "GpxIO::WriteRefPoint()"; )
	writer.writeStartElement("refpoint");
	writer.writeAttribute("lat",QString::number(r.Latitude()));
	writer.writeAttribute("lon",QString::number(r.Longitude()));
	writer.writeAttribute("x",QString::number(r.X()));
	writer.writeAttribute("y",QString::number(r.Y()));
	writer.writeEndElement();
}
