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


QMapSelectionDialog::QMapSelectionDialog(QMapList& maps, QWidget *parent)
    : QDialog(parent)
{
    QHBoxLayout *list = new QHBoxLayout();
    QHBoxLayout *buttons = new QHBoxLayout();
    QVBoxLayout *main = new QVBoxLayout(this);
    listWidget = new QListWidget();
    QPushButton *cancel = new QPushButton(tr("Cancel"));
    QPushButton *confirm = new QPushButton(tr("Confirm"));

    QStringList files = maps.keys();
    for (int i = 0; i < files.size(); ++i)
    {
        new QListWidgetItem(files[i], listWidget);
    }

    list->addWidget(listWidget);
    buttons->addWidget(confirm);
    buttons->addWidget(cancel);
    main->addLayout(list);
    main->addLayout(buttons);
    setLayout(main);

    listWidget->show();
    cancel->show();
    confirm->show();
    
    connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(confirm,SIGNAL(clicked()),this,SLOT(accept()));
    //connect(this,SIGNAL(accepted),this,SLOT(emitSelection()));
}

void QMapSelectionDialog::accept()
{
	QString filename = listWidget->currentItem()->text();
	emit selectmap(filename);
	QDialog::accept();
	close();
}

QMapSelectionDialog::~QMapSelectionDialog()
{
}

QMapWidget::QMapWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , latitude(0)
    , longitude(0)
    , cursor(QPoint(0,0))
    , scrolling(true)
    , meta(0)
{
    bgimage = new QImage(QString(UIDIR "map.svg"));
    mapimage = new QImage(QString(MAPDIR "51g_eindhoven.jpg"));
    meta = new QMapMetaData(QString(MAPDIR "51g_eindhoven.xml"));
    meta->SetSize(mapimage->width(),mapimage->height());
    meta->Calibrate();

    connect(this, SIGNAL(drag(int,int)), this, SLOT(moveMap(int,int)));
    connect(this, SIGNAL(singleTap()), this, SLOT(followGPSPosition()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(SelectMap()));

    CreateMapList();
}

QMapWidget::~QMapWidget()
{
}

void QMapWidget::CreateMapList()
{
    QDir directory = QDir(MAPDIR);

    QStringList files = directory.entryList(QStringList(QString("*.xml")),
                                 QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i)
    {
        LOG( "QMapWidget::CreateMapList() " << files[i].toStdString() << "\n"; )
        maplist[files[i]] = new QMapMetaData(QString(MAPDIR) + files[i]);
    }
}

void QMapWidget::SelectBestMapForCurrentPosition()
{
    LOG( "QMapWidget::SelectBestMapForCurrentPosition()\n"; )
    QStringList found;
    FindMapsForCurrentPosition(found);
    if (found.size() > 0)
    {
        LOG( "QMapWidget::SelectBestMapForCurrentPosition(): " << found[0].toStdString() << "\n"; )
        // for now select first entry
        LoadMap(found[0]);
    }
}

void QMapWidget::FindMapsForCurrentPosition(QStringList &found)
{
    LOG( "QMapWidget::FindMapsForCurrentPosition()\n"; )
    QStringList keys = maplist.keys();
    for (int i=0; i<keys.size(); ++i)
    {
        if (maplist[keys[i]]->IsPositionOnMap(latitude,longitude))
        {
            LOG( "QMapWidget::FindMapsForCurrentPosition(): " << keys[i].toStdString() << "\n"; )
            found.append(keys[i]);
        }
    }
}

void QMapWidget::LoadMap(QString filename)
{
    LOG( "QMapWidget::LoadMap(): " << filename.toStdString() << "\n"; )
    meta = maplist[filename];
    filename.remove(filename.size()-4,4);
    filename.append(".jpg");
    delete mapimage;
    mapimage = new QImage(QString(MAPDIR) + filename);
    meta->SetSize(mapimage->width(),mapimage->height());
    meta->Calibrate();
    update();
}

void QMapWidget::MapSelected(QListWidgetItem *item)
{
    QString filename = item->text();
    item->listWidget()->hide();
    LOG( "QMapWidget::MapSelected(): " << filename.toStdString() << "\n"; )
    LoadMap(filename);
}

void QMapWidget::SelectMap()
{
/*
    QListWidget *listWidget = new QListWidget(this);
    QStringList files = maplist.keys();
    for (int i = 0; i < files.size(); ++i)
    {
        new QListWidgetItem(files[i], listWidget);
    }
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(MapSelected(QListWidgetItem *)));
    listWidget->setGeometry(QRect(20,20, width()-40, height()-40));
    listWidget->show();
*/
    QMapSelectionDialog *dialog = new QMapSelectionDialog(maplist);
    connect(dialog,SIGNAL(selectmap(QString)),this,SLOT(LoadMap(QString)));
    dialog->setModal(true);
    dialog->showFullScreen();
}

void QMapWidget::updatePosition(double lat, double lon)
{
    //LOG( "QMapWidget::updatePosition()\n"; )
    latitude = lat;
    longitude = lon;
    if (scrolling) return;
    if (!meta->IsCalibrated()) return;
    if (!meta->IsPositionOnMap(lat,lon)) return;

    //LOG( "QMapWidget::updatePosition() OnMap!\n"; )
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
    if (!meta->IsPositionOnMap(latitude,longitude))
        SelectBestMapForCurrentPosition();
    updatePosition(latitude,longitude);
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
    source = QRectF(20+cursor.x()-w/2, 20+cursor.y()-h/2, w-40, h-40);
    target = QRectF(20, 20, w-40, h-40);
    painter.drawImage(target, *mapimage, source);

    if (scrolling || !meta->IsPositionOnMap(latitude,longitude))
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

