#include <QPainter>
#include <QXmlStreamReader>
#include "ui.h"
#include "qmapwidget.h"
#include <iostream>
using namespace std;

#ifdef Q_OS_SYMBIAN
#define MAPDIR "c:/data/qtracker/maps/"
#else
#define MAPDIR "/Users/hurenkam/workspace/qtracker/maps/"
#endif

/*
class Map:
    def __init__(self,name=None,filename=None,refpoints=[],size=(2582,1944)):
    # Size defaults to 5M, this is the max resolution of an N95 camera
    # larger values are not likely since the app would run out of RAM
        self.refpoints = refpoints
        self.size=size
        self.name=name
        self.filename=filename
        self.iscalibrated = False
        self.Calibrate()

    def PrintInfo(self):
        if self.size != None:
            print "Map %s (%i x %i)" % (self.name, self.size[0], self.size[1])
        else:
            print "Map %s (? x ?)" % self.name

        if self.iscalibrated:
            lat1,lon1,lat2,lon2 = self.area
            print "x2lon:%f y2lat:%f lon2x:%f lat2y:%f" % (self.x2lon, self.y2lat, self.lon2x, self.lat2y)
            print "Wgs84 topleft:     %f, %f" % (lat1,lon1)
            print "Wgs84 bottomright: %f, %f" % (lat2,lon2)

    def AddRefpoint(self,ref):
        self.refpoints.append(ref)
        self.Calibrate()

    def ClearRefpoints(self):
        self.refpoints = []
        self.iscalibrated = False

    def Calibrate(self):
        if self.refpoints != None and len(self.refpoints) > 1:

            r = self.refpoints
            found = False
            for i in range(0,len(r)):
                for j in range(0,len(r)):
                    if r[i].x != r[j].x and r[i].y != r[j].y \
                        and r[i].latitude != r[j].latitude and r[i].longitude != r[j].longitude:

                            r1 = r[i]
                            r2 = r[j]
                            found = True
                            break

            if not found:
                print "Refpoints available, but either dx or dy is 0"
                return

            dx = r2.x - r1.x
            dy = r2.y - r1.y
            dlon = r2.longitude - r1.longitude
            dlat = r2.latitude - r1.latitude

            theta = (math.atan2(dy,dx) * 180 / math.pi) + 90
            if theta > 180:
                theta -= 360
            d,b = r1.DistanceAndBearing(r2)
            dtheta = b - theta
            if dtheta > 180:
                dtheta -= 360

            self.x = r1.x
            self.y = r1.y
            self.lat = r1.latitude
            self.lon = r1.longitude
            try:
                self.x2lon = dlon/dx
                self.y2lat = dlat/dy
                self.lon2x = dx/dlon
                self.lat2y = dy/dlat
            except:
                print "Calibration failed for map ",self.name
                print "Refpoints: ",self.refpoints
                return

            self.iscalibrated = True
            self.area = self.WgsArea()
            #self.SaveCalibrationData()

    def WgsArea(self):
        if self.iscalibrated:
            lat1,lon1 = self.XY2Wgs(0,0)
            lat2,lon2 = self.XY2Wgs(self.size[0],self.size[1])
            return (lat1,lon1,lat2,lon2)

    def XY2Wgs(self,x,y):
        if self.iscalibrated:
            lon = (x - self.x)*self.x2lon + self.lon
            lat = (y - self.y)*self.y2lat + self.lat
            return lat,lon
        else:
            #print "Not calibrated"
            return None

    def Wgs2XY(self,lat,lon):
        if self.iscalibrated:
            x = (lon - self.lon)*self.lon2x + self.x
            y = (lat - self.lat)*self.lat2y + self.y
            return x,y
        else:
            #print "Not calibrated"
            return None

    def SetSize(self,size):
        self.size=size
        self.area = self.WgsArea()

    def PointOnMap(self,point):
        if self.size == None:
            return None

        if not self.iscalibrated:
            return None

        lat = point.latitude
        lon = point.longitude
        lat1,lon1,lat2,lon2 = self.area
        if lat > lat1 or lat < lat2 or lon < lon1 or lon > lon2:
            return None

        return self.Wgs2XY(point.latitude,point.longitude)
*/

QMapMetaData::QMapMetaData(QString filename)
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
                //cout << "Invalid Token: " << xml.errorString().toStdString() << "\n";
                break;
            case QXmlStreamReader::StartElement:
                if (xml.name().toString() == QString("map")) ReadMapElement(xml);
                if (xml.name().toString() == QString("resolution")) ReadResolutionElement(xml);
                if (xml.name().toString() == QString("refpoint")) ReadRefpointElement(xml);
                //cout << xml.name().toString().toStdString();
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
    width = attribs.value(QString(),QString("width")).toString().toDouble();
    height = attribs.value(QString(),QString("height")).toString().toDouble();
}

void QMapMetaData::ReadRefpointElement(QXmlStreamReader& xml)
{
    if ((count <0) || (count>9)) return;

    QXmlStreamAttributes attribs = xml.attributes();
    refpoints[count].setX(attribs.value(QString(),QString("x")).toString().toDouble());
    refpoints[count].setY(attribs.value(QString(),QString("y")).toString().toDouble());
    refpoints[count].setLatitude(attribs.value(QString(),QString("lat")).toString().toDouble());
    refpoints[count].setLongitude(attribs.value(QString(),QString("lon")).toString().toDouble());
    count++;
}

QMapMetaData::~QMapMetaData()
{
}



QMapWidget::QMapWidget(QWidget *parent)
    : QWidget(parent)
    , current(QPoint(0,0))
{
    bgimage = new QImage(QString(UIDIR "map.svg"));
    mapimage = new QImage(QString(MAPDIR "51g11_eindhoven.jpg"));
    QMapMetaData metadata(QString(MAPDIR "51g11_eindhoven.xml"));
}

QMapWidget::~QMapWidget()
{
}

void QMapWidget::mousePressEvent(QMouseEvent *event)
{
    //cout << "MousePressEvent " << event->pos().x() << " " << event->pos().y() << "\n";
    start = event->pos();
}

void QMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    //cout << "MouseDragEvent " << event->pos().x() << " " << event->pos().y() << "\n";
    stop = event->pos();
    moveMap(stop.x() - start.x(), stop.y() - start.y());
    start = event->pos();
}

void QMapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //cout << "MouseReleaseEvent " << event->pos().x() << " " << event->pos().y() << "\n";
    stop = event->pos();
    moveMap(stop.x() - start.x(), stop.y() - start.y());
}

void QMapWidget::moveMap(int x, int y)
{
    current.setX(current.x()-x);
    current.setY(current.y()-y);
    update();
}

void QMapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    double w = width();
    double h = height();

    QPainter painter(this);
    QRectF source(0, 0, 400, 360);
    QRectF target(0, 0, w, h);

    painter.drawImage(target, *bgimage, source);
    source = QRectF(current.x(), current.y(), w-40, h-40);
    target = QRectF(20, 20, w-40, h-40);
    painter.drawImage(target, *mapimage, source);
}

