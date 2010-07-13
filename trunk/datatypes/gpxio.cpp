#include <QFile>
#include <QXmlStreamReader>
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
}

void GpxIO::WriteRouteFile(const Route& r)
{
}

//========================================================================

void GpxIO::WriteGpxHeader()
{
}

void GpxIO::WriteTrack(Track& t)
{
}

void GpxIO::WriteRoute(Route& r)
{
}

void GpxIO::WriteWayPoint(WayPoint& w, QString tag)
{
}
