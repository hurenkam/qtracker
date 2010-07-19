#include <QFile>
#include <QXmlStreamReader>
#include <QString>
#include "gpxio.h"

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG2( a ) std::cout << a
#define LOG( a )

//========================================================================

GpxIO* GpxIO::instance = 0;

void GpxIO::ImportGpxFile(QString filename)
{
    LOG( "GpxIO::ImportGpxFile(" << filename.toStdString() << ")\n"; )
    
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	reader.setDevice(&file);

	while (!reader.atEnd())
	{
        LOG( "GpxIO::ImportGpxFile() xml.name(): " << reader.name().toString().toStdString() << "\n"; )
    
		if (reader.name()=="gpx")
			ReadGpx();
        
        reader.readNextStartElement();
	}
}

MapMetaData* GpxIO::ReadMapMetaFile(QString filename)
{
    LOG( "GpxIO::ReadMapMetaFile(" << filename.toStdString() << ")\n"; )

	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	reader.setDevice(&file);
	
    return ReadMapMetaData("map");
}

//========================================================================

void GpxIO::ReadGpx()
{
    LOG( "GpxIO::ReadGpx()\n"; )

	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "gpx"))
	{
        LOG( "GpxIO::ReadGpx() xml.name(): " << xml.name().toString().toStdString() << "\n"; )
    
        if (reader.tokenType()==QXmlStreamReader::StartElement)
        {
			//if (xml.name()=="time")       ReadElementString();
			//if (xml.name()=="bounds")     ReadBounds();
			if (reader.name()=="wpt")        wptlist.AddWayPoint(ReadWayPoint("wpt"));
			if (reader.name()=="rte")        rtelist->AddRoute(ReadRoute());
			if (reader.name()=="trk")        trklist->AddTrack(ReadTrack());
			if (reader.name()=="extensions") ReadExtensions();
        }
        reader.readNext();
	}
}

WayPoint* GpxIO::ReadWayPoint(QString tag="wpt")
{
    LOG( "GpxIO::ReadWayPoint()\n"; )
    
	WayPoint* wpt = new WayPoint();
	wpt->SetLatitude(ReadAttributeDouble("lat"));
	wpt->SetLongitude(ReadAttributeDouble("lon"));
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == tag))
	{
        LOG( "GpxIO::ReadWayPoint() readername(): " << reader.name().toString().toStdString() << "\n"; )
    
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
    LOG( "GpxIO::ReadWayPoint() " << tag.toStdString() << ": " << wpt.Name().toStdString() << ", " << wpt.Latitude() << ", " << wpt.Longitude() << "\n"; )
	return wpt;
}

Route* GpxIO::ReadRoute()
{
    LOG( "GpxIO::ReadRoute()\n"; )

	Route* rte = new Route();
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "rte"))
	{
        LOG( "GpxIO::ReadRoute() reader.name(): " << reader.name().toString().toStdString() << "\n"; )
    
        
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
    LOG( "GpxIO::ReadTrack()\n"; )

	Track* trk = new Track();
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "trk"))
	{
        LOG( "GpxIO::ReadTrack() reader.name(): " << reader.name().toString().toStdString() << "\n"; )
    
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
    LOG( "GpxIO::ReadExtensions()\n"; )

    while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "extensions"))
	{
        LOG( "GpxIO::ReadExtensions() reader.name(): " << reader.name().toString().toStdString() << "\n"; )
        
		if (reader.tokenType()==QXmlStreamReader::StartElement)
		{
			if (reader.name()=="mapmeta") maplist->AddMetaData(ReadMapMetaData("mapmeta"));
			if (reader.name()=="rtemeta") rtelist->AddMetaData(ReadAreaMetaData("rtemeta"));
			if (reader.name()=="trkmeta") trklist->AddMetaData(ReadAreaMetaData("trkmeta"));
		}
        reader.readNext();
	}
}

//========================================================================

AreaMetaData* GpxIO::ReadAreaMetaData(QString tag)
{
    LOG( "GpxIO::ReadAreaMetaData()\n"; )
	AreaMetaData* meta = new AreaMetaData();
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == tag))
	{
        LOG( "GpxIO::ReadAreaMetaData() reader.name():" << reader.name().toString().toStdString() << "\n"; )
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
    LOG( "GpxIO::ReadMapMetaData()\n"; )
	MapMetaData* meta = new MapMetaData();
	while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == tag))
	{
        LOG( "GpxIO::ReadMapMetaData() reader.name():" << reader.name().toString().toStdString() << "\n"; )
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
    LOG( "GpxIO::ReadResolution()\n"; )
	Resolution r = Resolution(
			ReadAttributeDouble("width"),
			ReadAttributeDouble("height")
		);
    LOG( "GpxIO::ReadResolution() result: " << r.Width() << ", " << r.Height() << "\n"; )
    return r;
}

RefPoint GpxIO::ReadRefPoint()
{
    LOG( "GpxIO::ReadRefPoint()\n"; )
	RefPoint r = RefPoint(
			ReadAttributeDouble("x"),
			ReadAttributeDouble("y"),
			ReadAttributeDouble("lat"),
			ReadAttributeDouble("lon")
		);
    LOG( "GpxIO::ReadRefPoint() result: " << r.X() << ", " << r.Y() << ", " << r.Latitude() << ", " << r.Longitude() << "\n"; )
    return r;
}

Bounds GpxIO::ReadBounds()
{
    LOG( "GpxIO::ReadBounds()\n"; )
	Bounds b = Bounds(
			ReadAttributeDouble("minlat"),
			ReadAttributeDouble("minlon"),
			ReadAttributeDouble("maxlat"),
			ReadAttributeDouble("maxlon")
		);
    LOG( "GpxIO::ReadBounds() result: " << b.MinLatitude() << ", " << b.MinLongitude() << ", " << b.MaxLatitude() << ", " << b.MaxLongitude() << "\n"; )
    return b;
}

//========================================================================

void GpxIO::WriteMapMetaFile(const MapMetaData& m)
{
}

void GpxIO::WriteTrackFile(const Track& t)
{
    LOG( "GpxIO::WriteTrackFile(" << t.FileName().toStdString() << ")\n"; )

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
    LOG( "GpxIO::WriteRouteFile(" << r.FileName().toStdString() << ")\n"; )

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
    LOG( "GpxIO::WriteGpxHeader()\n"; )
	writer.writeStartDocument();
	writer.writeStartElement("gpx");
	writer.writeAttribute("version","1.0");
	writer.writeAttribute("creator","qTracker r142 http://qtracker.googlecode.com");
	writer.writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
	writer.writeAttribute("xmlns","http://www.topografix.com/GPX/1/0");
	writer.writeAttribute("xsi:schemaLocation","http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd");
}

void GpxIO::WriteGpxFooter()
{
    LOG( "GpxIO::WriteGpxFooter()\n"; )
	writer.writeEndElement();
	writer.writeEndDocument();
}

void GpxIO::WriteTrack(const Track& t)
{
    LOG( "GpxIO::WriteTrack()\n"; )
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
    LOG( "GpxIO::WriteRoute()\n"; )
	writer.writeStartElement("rte");
	writer.writeEndElement();
}

void GpxIO::WriteWayPoint(const WayPoint& w, QString tag)
{
    LOG( "GpxIO::WriteWayPoint()\n"; )
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
