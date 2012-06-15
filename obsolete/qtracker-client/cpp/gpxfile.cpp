#include "gpxfile.h"
#include <QUrl>

//#define ENABLE_DEBUG
#include "helpers.h"

GpxFile::GpxFile(QObject *parent)
    : QObject(parent)
{
}

void GpxFile::setFileName(QVariant value)
{
    reset();
    filename = QUrl(value.toString()).toLocalFile();
    file.setFileName(filename);
    emit fileNameChanged(filename);
}

void GpxFile::reset()
{
    if (file.isOpen()) file.close();
}



void GpxFile::openGpx()
{
    file.open(QIODevice::ReadWrite);
    writer.setDevice(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("gpx");
    writer.writeAttribute("version","1.0");
    writer.writeAttribute("creator","qTracker v" VERSION " http://qtracker.googlecode.com");
    writer.writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    writer.writeAttribute("xmlns","http://www.topografix.com/GPX/1/0");
    writer.writeAttribute("xsi:schemaLocation","http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd");
}

void GpxFile::closeGpx()
{
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

void GpxFile::writePoint(const QString& tag, QVariantMap pt)
{
    writer.writeStartElement(tag);
    writer.writeAttribute("lat",QString::number(pt["latitude"].toDouble()));
    writer.writeAttribute("lon",QString::number(pt["longitude"].toDouble()));
    if (pt.contains("name") )
        writer.writeTextElement("name",pt["name"].toString());
    if (pt.contains("time"))
        writer.writeTextElement("time",pt["time"].toString());
    if (pt.contains("altitude"))
        writer.writeTextElement("ele",QString::number(pt["altitude"].toDouble()));
    writer.writeEndElement();
}

void GpxFile::writeWayPoint(QVariantMap wpt)
{
    writePoint("wpt",wpt);
}

void GpxFile::openRoute(QVariantMap rte)
{
    writer.writeStartElement("rte");
    if (rte.contains("name") )
        writer.writeTextElement("name",rte["name"].toString());
}

void GpxFile::closeRoute()
{
    writer.writeEndElement();
}

void GpxFile::writeRoutePoint(QVariantMap rtept)
{
    writePoint("rtept",rtept);
}

void GpxFile::openTrack(QVariantMap trk)
{
    writer.writeStartElement("rte");
    if (trk.contains("name") )
        writer.writeTextElement("name",trk["name"].toString());
}

void GpxFile::closeTrack()
{
    writer.writeEndElement();
}

void GpxFile::writeTrackPoint(QVariantMap trkpt)
{
    writePoint("rtept",trkpt);
}



void GpxFile::parseGpx()
{
    LOG( "GpxFile::parseFile(" << filename << ")" )

    file.open(QIODevice::ReadOnly);
    reader.setDevice(&file);

    while (!reader.atEnd())
    {
        LOG( "GpxFile::parseFile() xml.name(): " << reader.name() )
        if (reader.name()=="gpx")
        {
            while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "gpx"))
            {
                //LOG( "GpxFile::parseFile() xml.name(): " << reader.name() )

                if (reader.tokenType()==QXmlStreamReader::StartElement)
                {
                    //if (xml.name()=="time")       ReadElementString();
                    //if (xml.name()=="bounds")     ReadBounds();
                    if (reader.name()=="wpt")        parseWayPoint();
                    if (reader.name()=="rte")        parseRoute();
                    if (reader.name()=="trk")        parseTrack();
                    //if (reader.name()=="extensions") parseExtensions();
                }
                reader.readNext();
            }
        }
        if (reader.name()=="map")
        {
            while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "map"))
            {
                if (reader.tokenType()==QXmlStreamReader::StartElement)
                {
                    if (reader.name()=="resolution") parseResolution();
                    if (reader.name()=="refpoint")   parseRefpoint();
                }
                reader.readNext();
            }
        }

        reader.readNextStartElement();
    }
    file.close();
}

QVariant GpxFile::parsePoint(QString tag)
{
    LOG( "GpxFile::parseWaypoint(): " << reader.name() )

    QVariantMap pt;
    pt["latitude"]  = ReadAttributeDouble("lat");
    pt["longitude"] = ReadAttributeDouble("lon");

    while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == tag))
    {
        if (reader.tokenType()==QXmlStreamReader::StartElement)
        {
            if (reader.name()=="ele")  pt["altitude"]  = ReadElementDouble();
            if (reader.name()=="time") pt["time"]      = ReadElementString();
            if (reader.name()=="name") pt["name"]      = ReadElementString();
            //if (reader.name()=="desc") ;
            //if (reader.name()=="sym") ;
            //if (reader.name()=="type") ;
        }
        reader.readNext();
    }

    LOG( "GpxFile::parsePoint(" << tag << "): " << pt["name"].toString() << " " << pt["time"] << " " << pt["latitude"].toDouble() << " " << pt["longitude"].toDouble() )
    return pt;
}

void GpxFile::parseWayPoint()
{
    emit wayPoint(parsePoint("wpt"));
}

void GpxFile::parseRoute()
{
    LOG( "GpxFile::parseRoute(): " << reader.name() )
    emit routeFound();

    QVariantMap rte;
    while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "rte"))
    {
        if (reader.tokenType()==QXmlStreamReader::StartElement)
        {
            if (reader.name()=="name")  emit routeName( ReadElementString() );
            if (reader.name()=="rtept") emit routePoint( parsePoint("rtept") );
        }

        reader.readNext();
    }
    LOG( "GpxFile::parseRoute(): " << rte["name"].toString() )
}

void GpxFile::parseTrack()
{
    LOG( "GpxFile::parseTrack(): " << reader.name() )
    emit trackFound();

    QVariantMap trk;
    while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "trk"))
    {
        if (reader.tokenType()==QXmlStreamReader::StartElement)
        {
            if (reader.name()=="name")  emit trackName( ReadElementString() );
            if (reader.name()=="trkpt") emit trackPoint( parsePoint("trkpt") );
        }

        reader.readNext();
    }
    LOG( "GpxFile::parseTrack(): " << trk["name"].toString() )
}

void GpxFile::parseResolution()
{
    LOG( "GpxFile::parseResolution(): " << reader.name() )

    QVariantMap result;
    result["width"]  = ReadAttributeInt("width");
    result["height"] = ReadAttributeInt("height");

    LOG( "GpxFile::parseRefpoint(): " << result["width"].toInt() << "," << result["height"].toInt() )
    emit resolution(result);
}

void GpxFile::parseRefpoint()
{
    LOG( "GpxFile::parseRefpoint(): " << reader.name() )

    QVariantMap pt;
    pt["latitude"]  = ReadAttributeDouble("lat");
    pt["longitude"] = ReadAttributeDouble("lon");
    pt["x"] = ReadAttributeDouble("x");
    pt["y"] = ReadAttributeDouble("y");

    LOG( "GpxFile::parseRefpoint(): " << pt["latitude"].toDouble() << "," << pt["longitude"].toDouble() << " - " << pt["x"].toInt() << "," << pt["y"].toInt())
    emit refPoint(pt);
}
