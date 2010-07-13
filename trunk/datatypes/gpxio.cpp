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
	xml.setDevice(&file);

	while (!xml.atEnd())
	{
        LOG( "GpxIO::ImportGpxFile() xml.name(): " << xml.name().toString().toStdString() << "\n"; )
    
		if (xml.name()=="gpx")
			ReadGpx();
        
		xml.readNextStartElement();
	}
}

MapMetaData* GpxIO::ReadMapMetaFile(QString filename)
{
    LOG( "GpxIO::ReadMapMetaFile(" << filename.toStdString() << ")\n"; )

	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	xml.setDevice(&file);
	
    return ReadMapMetaData("map");
}

void GpxIO::WriteMapMetaFile(const MapMetaData& m)
{
}

//========================================================================

void GpxIO::ReadGpx()
{
    LOG( "GpxIO::ReadGpx()\n"; )

	while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "gpx"))
	{
        LOG( "GpxIO::ReadGpx() xml.name(): " << xml.name().toString().toStdString() << "\n"; )
    
        if (xml.tokenType()==QXmlStreamReader::StartElement)
        {
			//if (xml.name()=="time")       ReadElementString();
			//if (xml.name()=="bounds")     ReadBounds();
			if (xml.name()=="wpt")        wptlist.AddWayPoint(ReadWayPoint("wpt"));
			if (xml.name()=="rte")        rtelist->AddRoute(ReadRoute());
			if (xml.name()=="trk")        trklist->AddTrack(ReadTrack());
			if (xml.name()=="extensions") ReadExtensions();
        }
        xml.readNext();
	}
}

WayPoint* GpxIO::ReadWayPoint(QString tag="wpt")
{
    LOG( "GpxIO::ReadWayPoint()\n"; )
    
	WayPoint* wpt = new WayPoint();
	wpt->SetLatitude(ReadAttributeDouble("lat"));
	wpt->SetLongitude(ReadAttributeDouble("lon"));
	while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == tag))
	{
        LOG( "GpxIO::ReadWayPoint() xml.name(): " << xml.name().toString().toStdString() << "\n"; )
    
		if (xml.tokenType()==QXmlStreamReader::StartElement)
		{
			if (xml.name()=="ele")  wpt->SetElevation(ReadElementDouble());
			if (xml.name()=="time") wpt->SetTime(ReadElementString());
			if (xml.name()=="name") wpt->SetName(ReadElementString());
			//if (xml.name()=="desc") ;
			//if (xml.name()=="sym") ;
			//if (xml.name()=="type") ;
		}
		xml.readNext();
	}
    LOG( "GpxIO::ReadWayPoint() " << tag.toStdString() << ": " << wpt.Name().toStdString() << ", " << wpt.Latitude() << ", " << wpt.Longitude() << "\n"; )
	return wpt;
}

Route* GpxIO::ReadRoute()
{
    LOG( "GpxIO::ReadRoute()\n"; )

	Route* rte = new Route();
	while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "rte"))
	{
        LOG( "GpxIO::ReadRoute() xml.name(): " << xml.name().toString().toStdString() << "\n"; )
    
        
		if (xml.tokenType()==QXmlStreamReader::StartElement)
		{
			if (xml.name()=="name")  rte->SetName(ReadElementString());
			if (xml.name()=="rtept") rte->AddPoint(ReadWayPoint("rtept"));
		}
		
		xml.readNext();
	}
	return rte;
}

Track* GpxIO::ReadTrack()
{
    LOG( "GpxIO::ReadTrack()\n"; )

	Track* trk = new Track();
	while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "trk"))
	{
        LOG( "GpxIO::ReadTrack() xml.name(): " << xml.name().toString().toStdString() << "\n"; )
    
		if (xml.tokenType()==QXmlStreamReader::StartElement)
		{
			if (xml.name()=="name")  trk->SetName(ReadElementString());
			if (xml.name()=="trkpt") trk->AddPoint(ReadWayPoint("trkpt"));
		}
        xml.readNext();
	}
	return trk;
}

void GpxIO::ReadExtensions()
{
    LOG( "GpxIO::ReadExtensions()\n"; )

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "extensions"))
	{
        LOG( "GpxIO::ReadExtensions() xml.name(): " << xml.name().toString().toStdString() << "\n"; )
        
		if (xml.tokenType()==QXmlStreamReader::StartElement)
		{
			if (xml.name()=="mapmeta") maplist->AddMetaData(ReadMapMetaData("mapmeta"));
			if (xml.name()=="rtemeta") rtelist->AddMetaData(ReadAreaMetaData("rtemeta"));
			if (xml.name()=="trkmeta") trklist->AddMetaData(ReadAreaMetaData("trkmeta"));
		}
        xml.readNext();
	}
}

//========================================================================

AreaMetaData* GpxIO::ReadAreaMetaData(QString tag)
{
    LOG( "GpxIO::ReadAreaMetaData()\n"; )
	AreaMetaData* meta = new AreaMetaData();
	while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == tag))
	{
        LOG( "GpxIO::ReadAreaMetaData() xml.name():" << xml.name().toString().toStdString() << "\n"; )
		if (xml.tokenType()==QXmlStreamReader::StartElement)
		{
			if (xml.name()=="name")    meta->SetName(ReadElementString());
			if (xml.name()=="bounds")  meta->SetBounds(ReadBounds());
		}
		xml.readNext();
	}
	return meta;
}

MapMetaData* GpxIO::ReadMapMetaData(QString tag)
{
    LOG( "GpxIO::ReadMapMetaData()\n"; )
	MapMetaData* meta = new MapMetaData();
	while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == tag))
	{
        LOG( "GpxIO::ReadMapMetaData() xml.name():" << xml.name().toString().toStdString() << "\n"; )
		if (xml.tokenType()==QXmlStreamReader::StartElement)
		{
			if (xml.name()=="resolution") meta->SetResolution(ReadResolution());
			if (xml.name()=="refpoint")   meta->AddRefPoint(ReadRefPoint());
			if (xml.name()=="name")       meta->SetName(ReadElementString());
		}
		xml.readNext();
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
