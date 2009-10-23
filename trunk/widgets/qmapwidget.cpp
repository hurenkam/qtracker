#include <QPainter>
#include <QFlags>
#include <QIcon>
#include <cmath>
#include "ui.h"
#include "qmapwidget.h"
#include "qmapselectiondialog.h"

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG( a )

static double zoomlevels[] = { 0.33, 0.5, 0.66, 1.0, 1.41, 2.0, 3.0 };
const int     zoomneutral = 3;
const int	  zoommax = 6;

#ifdef Q_OS_SYMBIAN
#define MAPDIR "c:/data/qtracker/maps/"
#else
#define MAPDIR "/Users/hurenkam/workspace/qtracker/maps/"
#endif

QMapWidget::QMapWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , state(StNoMap)
    , zoom(zoomneutral)
    , x(0)
    , y(0)
    , latitude(0)
    , longitude(0)
    , meta(0)
    , mapimage(0)
    , bgimage(new QImage(QString(UIDIR "map.svg")))
    , svgZoomIn(new QImage(QString(UIDIR "zoom-in.svg")))
    , svgZoomOut(new QImage(QString(UIDIR "zoom-out.svg")))
    , svgOptions(new QImage(QString(UIDIR "options.svg")))
    , svgHome(new QImage(QString(UIDIR "home.svg")))
    , svgBar(new QImage(QString(UIDIR "statusbar.svg")))
    , zooming(0)
    , mapname("<no map loaded>")
{
    connect(this, SIGNAL(drag(int,int)), this, SLOT(moveMap(int,int)));
    connect(this, SIGNAL(singleTap()), this, SLOT(FollowGPS()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(SelectMapForCurrentPosition()));
    CreateMapList();
    connect(&zoomtimer,SIGNAL(timeout()),this,SLOT(zoomRepeat()));
    zoomtimer.setInterval(150);
    connect(this, SIGNAL(zoomin()), this, SLOT(zoomIn()));
    connect(this, SIGNAL(zoomout()), this, SLOT(zoomOut()));
    connect(this, SIGNAL(options()), this, SLOT(SelectMap()));  // to be menu
    connect(this, SIGNAL(home()), this, SLOT(SelectMapForCurrentPosition()));
}

QMapWidget::~QMapWidget()
{
/*
    delete mapimage;
    mapimage = 0;
    meta = 0;
    QStringList names = maplist.keys();
    for (int i; i<maplist.size(); ++i)
        delete maplist[names[i]];
*/
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

bool QMapWidget::SelectBestMapForCurrentPosition()
{
    LOG( "QMapWidget::SelectBestMapForCurrentPosition()\n"; )
    QStringList found;
    FindMapsForCurrentPosition(found);
    if (found.size() > 0)
    {
        LOG( "QMapWidget::SelectBestMapForCurrentPosition(): " << found[0].toStdString() << "\n"; )
        // for now select first entry
        int index = 0;
        int lon2x = maplist[found[0]]->Lon2x();
        for (int i=1; i<found.size(); ++i)
            if (maplist[found[i]]->Lon2x() > lon2x)
            {
                lon2x = maplist[found[i]]->Lon2x();
                index = i;
            }
        return LoadMap(found[index]);
    }
    return false;
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

bool QMapWidget::LoadMap(QString filename)
{
    LOG( "QMapWidget::LoadMap(): " << filename.toStdString() << "\n"; )
    meta = maplist[filename];
    filename.remove(filename.size()-4,4);
    filename.append(".jpg");
    if (mapimage)
        delete mapimage;
    mapimage = new QImage();
    bool result = mapimage->load(QString(MAPDIR) + filename);
    if (!result)
    {
            QMessageBox msg;
            msg.setText(QString("Unable to load map ") + filename);
            msg.setIcon(QMessageBox::Warning);
            msg.exec();
            mapname = QString("<no map loaded>");
    }
    else
    {
            meta->SetSize(mapimage->width(),mapimage->height());
            meta->Calibrate();
            x = mapimage->width()/2;
            y = mapimage->height()/2;
            mapname = filename;
    }
    update();
    return result;
}

void QMapWidget::MapSelected(QString map)
{
    if (LoadMap(map))
        state = StScrolling;
    else
        state = StNoMap;
}

void QMapWidget::SelectMap()
{
    QStringList files = maplist.keys();
    QMapSelectionDialog *dialog = new QMapSelectionDialog(files);
    connect(dialog,SIGNAL(selectmap(QString)),this,SLOT(MapSelected(QString)));
    dialog->setModal(true);
#ifdef Q_OS_SYMBIAN
    dialog->showFullScreen();
#else
    dialog->show();
#endif
}

void QMapWidget::SelectMapForCurrentPosition()
{
    QStringList files;
    FindMapsForCurrentPosition(files);
    
    if (files.length() > 0)
    {
		QMapSelectionDialog *dialog = new QMapSelectionDialog(files);
		connect(dialog,SIGNAL(selectmap(QString)),this,SLOT(MapSelected(QString)));
		dialog->setModal(true);
	#ifdef Q_OS_SYMBIAN
		dialog->showFullScreen();
	#else
		dialog->show();
	#endif
    }
    else
	{
		QMessageBox msg;
		msg.setText(QString("No maps available for this location"));
		msg.setIcon(QMessageBox::Warning);
		msg.exec();
	}
}

void QMapWidget::updatePosition(double lat, double lon)
{
    //LOG( "QMapWidget::updatePosition()\n"; )
    latitude = lat;
    longitude = lon;
    if (state == StFollowGPS)
        FollowGPS();
    else
        update();
}

void QMapWidget::moveMap(int dx, int dy)
{
    x -= dx * zoomlevels[zoom];
    y -= dy * zoomlevels[zoom];
    if (state == StFollowGPS)
        state = StScrolling;
    update();
}

void QMapWidget::FollowGPS()
{
    LOG( "QMapWidget::FollowGPS()\n"; )

    if (IsPositionOnMap())
    {   // OnMap
        SetCursorToCurrentPosition();
        state = StFollowGPS;
    }
    else
    {   // OffMap
        QStringList foundmaps;
        FindMapsForCurrentPosition(foundmaps);
        if (foundmaps.size() > 0)
        {
            // MapAvailable
            if (SelectBestMapForCurrentPosition())
                // Load Succeeded
                state = StFollowGPS;
            else
                // Load Failed
                state = StNoMap;
        }
        else
        {
            // NoMapAvailable
            if (mapimage)
                // KeepPreviousMap
                state = StFollowGPS;
            else
                // NoPreviousMap
                state = StNoMap;
        }
    }
    update();
}

void QMapWidget::zoomRepeat()
{
    zoom += zooming;
    if (zoom > zoommax) zoom = zoommax;
    if (zoom < 0) zoom = 0;
    update();
}

void QMapWidget::zoomIn()
{
    zooming = -1;
    zoomRepeat();
    zoomtimer.start();
}

void QMapWidget::zoomOut()
{
    zooming = +1;
    zoomRepeat();
    zoomtimer.start();
}

void QMapWidget::mousePressEvent(QMouseEvent *event)
{
    if ((event->pos().x() > width()-75) && (event->pos().y() < 75)) emit zoomin();
    else if ((event->pos().x() > width()-75) && (event->pos().y() > height()-75)) emit zoomout();
    else if ((event->pos().x() < 75) && (event->pos().y() < 75)) emit home();
    else if ((event->pos().x() < 75) && (event->pos().y() > height()-75)) emit options();
    else
        QGaugeWidget::mousePressEvent(event);
}

void QMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (zooming == 0)
                QGaugeWidget::mouseMoveEvent(event);
}

void QMapWidget::mouseReleaseEvent(QMouseEvent *event)
{
        if (zooming == 0)
        {
                QGaugeWidget::mouseReleaseEvent(event);
        }
        zooming = 0;
        zoomtimer.stop();
}

void QMapWidget::paintEvent(QPaintEvent *event)
{
        QWidget::paintEvent(event);

    double w = width();
    double h = height();
    double s = h / 36;

    QPainter painter(this);
    QRectF source(0, 0, 400, 360);
    QRectF target(-w/2, -h/2, w, h);
    painter.setWindow(-w/2,-h/2,w,h);

    painter.drawImage(target, *bgimage, source);
    painter.setViewport(20,20,w-40,h-40);
    if (mapimage)
    {
        double z = zoomlevels[zoom];
        source = QRectF(w*z/-2 + x, h*z/-2 + y, w*z, h*z);
        target = QRectF(w*z/-2, h*z/-2, w*z, h*z);
        painter.setWindow(-w/2*z,-h/2*z,w*z,h*z);
        painter.drawImage(target, *mapimage, source);
        painter.setWindow(-w/2,-h/2,w,h);
    }
    source = QRectF(0,0,48,48);
    target = QRectF(w/2-48,h/-2,48,48);
    painter.setViewport(12,12,w-24,h-24);
    painter.drawImage(target, *svgZoomIn, source);
    target = QRectF(w/2-48,h/2-48,48,48);
    painter.drawImage(target, *svgZoomOut, source);
    target = QRectF(w/-2,h/2-48,48,48);
    painter.drawImage(target, *svgOptions, source);
    target = QRectF(w/-2,h/-2,48,48);
    painter.drawImage(target, *svgHome, source);
    source = QRectF(0,0,300,48);
    target = QRectF(w/-2,h/2-48,300,48);
    painter.drawImage(target, *svgBar, source);

    char buf[25];
    sprintf(buf,"%s",mapname.toStdString().c_str());
    painter.setFont(QFont("Courier", 168/TEXTDIVIDER));
    QRect r = painter.boundingRect(w/-2+58,h/2-38,260,28, Qt::AlignLeft, buf);
    painter.setPen(QPen(Qt::blue));
    painter.drawText(r, Qt::AlignLeft, buf);

    sprintf(buf,"%08.5fN %08.5fE",latitude,longitude);
    r = painter.boundingRect(w/-2+58,h/2-25,260,28, Qt::AlignLeft, buf);
    painter.drawText(r, Qt::AlignLeft, buf);

    if ((state == StScrolling) || (!IsPositionOnMap()))
    {
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
    }
    else
    {
        painter.setPen(Qt::green);
        painter.setBrush(Qt::green);
    }
    painter.drawEllipse(s/-2,s/-2,s,s);
}
