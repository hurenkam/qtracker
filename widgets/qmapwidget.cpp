#include <QPainter>
#include <QXmlStreamReader>
#include <QString>
#include <QFileDialog>
#include "ui.h"
#include "qmapwidget.h"
#include <cmath>

#include <iostream>
//using namespace std;
//#define LOG( a ) std::cout << a
#define LOG( a )


#ifdef Q_OS_SYMBIAN
#define MAPDIR "c:/data/qtracker/maps/"
#else
#define MAPDIR "/Users/hurenkam/workspace/qtracker/maps/"
#endif

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
    QXmlStreamReader xml;
    QFile file(filename);
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
}

void QMapMetaData::ReadMapElement(QXmlStreamReader& xml)
{
    QXmlStreamAttributes attribs = xml.attributes();
    filename = attribs.value(QString(),QString("imagefile")).toString();
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

bool QMapMetaData::IsPositionOnMap(double alat, double alon)
{
    LOG( "QMapMetaData::IsPositionOnMap(" << alat << "," << alon << ")\n"; )
    if ((width == 0) || (height == 0) || (!iscalibrated)) return false;

    double lat1,lat2,lon1,lon2;
    WgsArea(lat1,lon1,lat2,lon2);
    LOG( "QMapMetaData::IsPositionOnMap(): " << lat1 << "," << lon1 << "..." << lat2 << "," << lon2 << "\n"; )

    if (lat1 > lat2)
        if ((alat < lat2) || (alat > lat1))
        {
            LOG( "QMapMetaData::IsPositionOnMap(): fail1 \n"; )
            return false;
        }

    if (lat2 > lat1)
        if ((alat < lat1) || (alat > lat2))
        {
            LOG( "QMapMetaData::IsPositionOnMap(): fail2 " << alat << "<" << lat1 << " or " << alat <<">" << lat2 << "\n"; )
            return false;
        }

    if (lon1 > lon2)
        if ((alon < lon2) || (alon > lon1))
        {
            LOG( "QMapMetaData::IsPositionOnMap(): fail3 \n"; )
            return false;
        }

    if (lon2 > lon1)
        if ((alon < lon1) || (alon > lon2))
        {
            LOG( "QMapMetaData::IsPositionOnMap(): fail4 \n"; )
            return false;
        }

    LOG( "QMapMetaData::IsPositionOnMap(): Yes!\n"; )
    return true;
}

QMapMetaData::~QMapMetaData()
{
}



QMapWidget::QMapWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , cursor(QPoint(0,0))
    , position(QPointF(0,0))
    , scrolling(true)
    , meta(0)
    , onmap(false)
{
    bgimage = new QImage(QString(UIDIR "map.svg"));
    mapimage = new QImage(QString(MAPDIR "nederland.jpg"));
    meta = new QMapMetaData(QString(MAPDIR "nederland.xml"));
    meta->SetSize(mapimage->width(),mapimage->height());
    meta->Calibrate();

    connect(this, SIGNAL(drag(int,int)), this, SLOT(moveMap(int,int)));
    connect(this, SIGNAL(singleTap()), this, SLOT(followGPSPosition()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(openMap()));
}

QMapWidget::~QMapWidget()
{
}

void QMapWidget::openMap()
{
    QString filename = QFileDialog::getOpenFileName(this,
    		tr("Open Map"),MAPDIR,
    		tr("Calibrated (*.xml *.mcx *.ozi);;Uncalibrated (*.jpg)")
    		);
    //if (meta) delete meta;
    if (mapimage) delete mapimage;
    mapimage = new QImage(filename);
    meta->SetSize(mapimage->width(),mapimage->height());
    update();
}

void QMapWidget::updatePosition(double lat, double lon)
{
    LOG( "QMapWidget::updatePosition()\n"; )
    if (scrolling) return;
    if (!meta->IsCalibrated()) return;
    
    onmap = meta->IsPositionOnMap(lat,lon);
    if ( !onmap ) return;

    LOG( "QMapWidget::updatePosition() OnMap!\n"; )
    double x,y;
    meta->Wgs2XY(lat,lon,x,y);
    cursor.setX(x);
    cursor.setY(y);
    update();
}

void QMapWidget::moveMap(int x, int y)
{
    scrolling = true;
    cursor.setX(cursor.x()-x);
    cursor.setY(cursor.y()-y);
    update();
}

void QMapWidget::followGPSPosition()
{
    LOG( "QMapWidget::followGPSPosition()\n"; )
    scrolling = false;
    update();
}

void QMapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    double w = width();
    double h = height();
    double s = h / 36;

    QPainter painter(this);
    QRectF source(0, 0, 400, 360);
    QRectF target(0, 0, w, h);

    painter.drawImage(target, *bgimage, source);
    source = QRectF(cursor.x(), cursor.y(), w-40, h-40);
    target = QRectF(20, 20, w-40, h-40);
    painter.drawImage(target, *mapimage, source);
    
    if (!onmap)
    {
		painter.setPen(Qt::red);
		painter.setBrush(Qt::red);
    }
    else
    {
		painter.setPen(Qt::green);
		painter.setBrush(Qt::green);
    }
    painter.translate(w/2,h/2);
    painter.drawEllipse(s/-2,s/-2,s,s);
}

