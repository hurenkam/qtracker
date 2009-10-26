#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QString>
#include "qmapmetadata.h"

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG( a )

const float PI = 3.14159265358979323846f;

static void CalculateDistanceAndBearing(
                double fromlat,
                double fromlon,
                double tolat,
                double tolon,
                double &distance,
                double &bearing)
{
    double earths_radius = (6378137.0 + 6356752.3141) / 2.0;

    double from_theta = float(fromlat) / 360.0 * 2.0 * PI;
    double from_landa = float(fromlon) / 360.0 * 2.0 * PI;
    double to_theta = float(tolat)     / 360.0 * 2.0 * PI;
    double to_landa = float(tolon)     / 360.0 * 2.0 * PI;

    distance = acos(
            sin(from_theta) * sin(to_theta) +
            cos(from_theta) * cos(to_theta) * cos(to_landa-from_landa)
                ) * earths_radius;

    bearing = atan2(
                sin(to_landa-from_landa) * cos(to_theta),
                cos(from_theta) * sin(to_theta) -
                sin(from_theta) * cos(to_theta) * cos(to_landa-from_landa)
            );
    bearing = bearing / 2.0 / PI * 360.0;
}

QMapMetaData::QMapMetaData(QString filename)
    : iscalibrated(false)
    , count(0)
{
    metafilename = filename;
    QXmlStreamReader xml;
    QFile file(metafilename);
    QXmlStreamReader::TokenType tt;
    file.open(QIODevice::ReadOnly);
    file.readLine();

    xml.setDevice(&file);

    while (!xml.atEnd()) {
        tt = xml.readNext();
        switch (tt)
        {
            case QXmlStreamReader::Invalid:
                LOG( "QMapMetaData::QMapMetaData(): Invalid token!" << xml.errorString().toStdString() << "\n"; )
                break;
            case QXmlStreamReader::StartElement:
                LOG( "QMapMetaData::QMapMetaData(): StartElement " << xml.name().toString().toStdString() << "\n"; )
                if (xml.name().toString() == QString("map")) ReadMapElement(xml);
                if (xml.name().toString() == QString("resolution")) ReadResolutionElement(xml);
                if (xml.name().toString() == QString("refpoint")) ReadRefpointElement(xml);
                break;
            case QXmlStreamReader::NoToken:
            case QXmlStreamReader::StartDocument:
            case QXmlStreamReader::EndDocument:
            case QXmlStreamReader::EndElement:
            case QXmlStreamReader::Characters:
            case QXmlStreamReader::Comment:
            case QXmlStreamReader::DTD:
            case QXmlStreamReader::EntityReference:
            case QXmlStreamReader::ProcessingInstruction:
                break;
        }
    }
    file.close();
    Calibrate();
}

bool QMapMetaData::AddRefPoint(double lat, double lon, int x, int y)
{
	if ((count <0) || (count>9)) return false;
	
	refpoints[count].setX(x);
	refpoints[count].setY(y);
	refpoints[count].setLatitude(lat);
	refpoints[count].setLongitude(lon);
	LOG( "QMapMetaData::AddRefpoint("<< count << "): " << refpoints[count].X() << " " << refpoints[count].Y() << " "\
		 << refpoints[count].Latitude() << " " << refpoints[count].Longitude() << "\n"; )
	count++;
	
	Save();
	return true;
}

void QMapMetaData::SaveResolutionElement(QXmlStreamWriter& xml)
{
	xml.writeStartElement("resolution");
	xml.writeAttribute("width",QVariant(width).toString());
	xml.writeAttribute("height",QVariant(height).toString());
	xml.writeEndElement(); // resolution
}

void QMapMetaData::SaveRefpointElement(QXmlStreamWriter& xml, RefPoint& r)
{
	xml.writeStartElement("refpoint");
	xml.writeAttribute("lat",QVariant(r.Latitude()).toString());
	xml.writeAttribute("lon",QVariant(r.Longitude()).toString());
	xml.writeAttribute("x",QVariant(r.X()).toString());
	xml.writeAttribute("y",QVariant(r.Y()).toString());
	xml.writeEndElement(); // refpoint
}

void QMapMetaData::SaveMapElement(QXmlStreamWriter& xml)
{
	xml.writeStartElement("map");
	xml.writeAttribute("imagefile",imagefilename);
	SaveResolutionElement(xml);
	for (int i =0; i<count; ++i)
	{
		SaveRefpointElement(xml,refpoints[i]);
	}
	xml.writeEndElement(); // map
}

void QMapMetaData::Save()
{
	QXmlStreamWriter xml;
	QFile file(metafilename);
	file.open(QIODevice::WriteOnly);
    xml.setDevice(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument("1.0");
    SaveMapElement(xml);
    xml.writeEndDocument();
	file.close();
}

void QMapMetaData::ReadMapElement(QXmlStreamReader& xml)
{
    QXmlStreamAttributes attribs = xml.attributes();
    imagefilename = attribs.value(QString(),QString("imagefile")).toString();
}

void QMapMetaData::ReadResolutionElement(QXmlStreamReader& xml)
{
    QXmlStreamAttributes attribs = xml.attributes();
    width = int(attribs.value(QString(),QString("width")).toString().toDouble());
    height = int(attribs.value(QString(),QString("height")).toString().toDouble());
}

void QMapMetaData::ReadRefpointElement(QXmlStreamReader& xml)
{
    if ((count <0) || (count>9)) return;

    QXmlStreamAttributes attribs = xml.attributes();
    refpoints[count].setX(int(attribs.value(QString(),QString("x")).toString().toDouble()));
    refpoints[count].setY(int(attribs.value(QString(),QString("y")).toString().toDouble()));
    refpoints[count].setLatitude(attribs.value(QString(),QString("lat")).toString().toDouble());
    refpoints[count].setLongitude(attribs.value(QString(),QString("lon")).toString().toDouble());
    LOG( "QMapMetaData::ReadRefpointElement("<< count << "): " << refpoints[count].X() << " " << refpoints[count].Y() << " "\
         << refpoints[count].Latitude() << " " << refpoints[count].Longitude() << "\n"; )
    count++;
}

void QMapMetaData::CalculateIndexesFromRefpoints(int i, int j)
{
    LOG( "QMapMetaData::CalculateIndexesFromRefpoints(" << i << "," << j << ")\n"; )
    int dx = refpoints[j].X() - refpoints[i].X();
    int dy = refpoints[j].Y() - refpoints[i].Y();
    double dlon = refpoints[j].Longitude() - refpoints[i].Longitude();
    double dlat = refpoints[j].Latitude() - refpoints[i].Latitude();
    LOG( "QMapMetaData::CalculateIndexesFromRefpoints(): " << dx << " " << dy << " " << dlon << " " << dlat << "\n"; )

    double theta = (atan2(dy,dx) * 180 / PI) + 90;
    if (theta > 180)
        theta -= 360;

    double d,b;
    CalculateDistanceAndBearing(
        refpoints[i].Latitude(),refpoints[i].Longitude(),
        refpoints[j].Latitude(),refpoints[j].Longitude(),
        d,b);

    double dtheta = b - theta;
    if (dtheta > 180)
        dtheta -= 360;

    x = refpoints[i].X();
    y = refpoints[i].Y();
    lat = refpoints[i].Latitude();
    lon = refpoints[i].Longitude();
    x2lon = dlon/dx;
    y2lat = dlat/dy;
    lon2x = dx/dlon;
    lat2y = dy/dlat;
    iscalibrated = true;
    LOG( "QMapMetaData::CalculateIndexesFromRefpoints(): " << x << " " << y << " " << lat << " " << lon << "\n"; )
    LOG( "QMapMetaData::CalculateIndexesFromRefpoints(): " << x2lon << " " << y2lat << " " << lat2y << " " << lon2x << "\n"; )
}

bool QMapMetaData::IsValidRefpointPair(int i, int j)
{
    if (i == j) return false;
    LOG( "QMapMetaData::IsValidRefpointPair(" << i << "," << j << ")\n"; )

    if ((refpoints[i].X() != refpoints[j].X()) &&
        (refpoints[i].Y() != refpoints[j].Y()) &&
        (refpoints[i].Latitude() != refpoints[j].Latitude()) &&
        (refpoints[i].Longitude() != refpoints[j].Longitude()))
        return true;

    LOG( "QMapMetaData::IsValidRefpointPair(): No!\n"; )
    return false;
}

void QMapMetaData::Calibrate()
{
    LOG( "QMapMetaData::Calibrate()\n"; )
    if (count < 2) return;

    int i=0;
    int j=0;
    bool found = false;
    while (!found && (i<count))
    {
        while (!found && (j < count))
        {
            found = IsValidRefpointPair(i,j);
            j++;
        }
        i++;
    }

    if (!found) return;

    CalculateIndexesFromRefpoints(--i,--j);
}

bool QMapMetaData::WgsArea(double& lat1, double& lon1, double& lat2, double& lon2)
{
    if (!iscalibrated) return false;
    //LOG( "QMapMetaData::WgsArea(): " << width << " " << height << "\n"; )

    XY2Wgs(0,0,lat1,lon1);
    XY2Wgs(width,height,lat2,lon2);

    return true;
}

bool QMapMetaData::XY2Wgs(double ax, double ay, double& alat, double& alon)
{
    if (!iscalibrated) return false;

    alon = (ax - x) * x2lon + lon;
    alat = (ay - y) * y2lat + lat;

    return true;
}

bool QMapMetaData::Wgs2XY(double alat, double alon, double& ax, double& ay)
{
    if (!iscalibrated) return false;

    ax = (alon - lon) * lon2x + x;
    ay = (alat - lat) * lat2y + y;
    return true;
}

static bool IsValueInRange(double v, double r1, double r2)
{
    if (r1 > r2)
        if ((v < r2) || (v > r1))
            return false;

    if (r2 > r1)
        if ((v < r1) || (v > r2))
            return false;

    return true;
}

bool QMapMetaData::IsPositionOnMap(double alat, double alon)
{
    //LOG( "QMapMetaData::IsPositionOnMap(" << alat << "," << alon << ")\n"; )
    if ((width == 0) || (height == 0) || (!iscalibrated)) return false;

    double lat1,lat2,lon1,lon2;
    WgsArea(lat1,lon1,lat2,lon2);

    if (!IsValueInRange(alat,lat1,lat2)) return false;
    if (!IsValueInRange(alon,lon1,lon2)) return false;

    LOG( "QMapMetaData::IsPositionOnMap(): Yes!\n"; )
    return true;
}

QMapMetaData::~QMapMetaData()
{
}
