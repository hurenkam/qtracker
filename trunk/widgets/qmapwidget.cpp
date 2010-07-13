#include <QPainter>
#include <QFlags>
#include <QIcon>
#include <cmath>
#include "ui.h"
#include "qmapwidget.h"
#include "qmapselectiondialog.h"
#include "qwaypointdialog.h"
#include "gpxio.h"

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG2( a ) std::cout << a
#define LOG( a )

static double zoomlevels[] = { 0.33, 0.5, 0.66, 1.0, 1.41, 2.0, 3.0 };
const int     zoomneutral = 3;
const int	  zoommax = 6;

#ifdef Q_OS_SYMBIAN
#define MAPDIR "/data/qtracker/maps/"
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
    , bgimage(new QImage(GetDrive() + QString(UIDIR "map.svg")))
    , svgZoomIn(new QImage(GetDrive() + QString(UIDIR "zoom-in.svg")))
    , svgZoomOut(new QImage(GetDrive() + QString(UIDIR "zoom-out.svg")))
    , svgOptions(new QImage(GetDrive() + QString(UIDIR "options.svg")))
    //, svgHome(new QImage(GetDrive() + QString(UIDIR "home.svg")))
    , svgFlag(new QImage(GetDrive() + QString(UIDIR "flag.svg")))
    , svgHiker(new QImage(GetDrive() + QString(UIDIR "hiker.svg")))
    , svgBar(new QImage(GetDrive() + QString(UIDIR "statusbar.svg")))
    , zooming(0)
    , mapname("<no map loaded>")
    , recordtrack(0)
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
    connect(this, SIGNAL(datum()), this, SLOT(SelectMap()));  // to be menu
    connect(this, SIGNAL(waypoint()), this, SLOT(SelectPoint()));
    connect(this, SIGNAL(track()), this, SLOT(StartTrack()));
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
        MapMetaData m = MapList::Instance()->GetItem(found[0]);
        int lon2x = m.Lon2x();
        for (int i=1; i<found.size(); ++i)
        {
        	m = MapList::Instance()->GetItem(found[i]);
        	if (m.Lon2x() > lon2x)
            {
                lon2x = m.Lon2x();
                index = i;
            }
        }
        return LoadMap(found[index]);
    }

    return false;
}

void QMapWidget::FindMapsForCurrentPosition(QStringList &found)
{
    LOG( "QMapWidget::FindMapsForCurrentPosition()\n"; )

    QStringList keys = MapList::Instance()->MapNames();
    for (int i=0; i<keys.size(); ++i)
    {
        if (MapList::Instance()->GetItem(keys[i]).IsPositionOnMap(latitude,longitude))
        {
            LOG( "QMapWidget::FindMapsForCurrentPosition(): " << keys[i].toStdString() << "\n"; )
            found.append(keys[i]);
        }
    }
}

bool QMapWidget::LoadMap(QString filename)
{
    LOG( "QMapWidget::LoadMap(): " << filename.toStdString() << "\n"; )
    //QString fullpath = GetDrive() + QString(MAPDIR) + filename + QString(".jpg");

    if (mapimage)
        delete mapimage;

    mapimage = new QImage();
    meta = &MapList::Instance()->GetItem(filename);
    bool result = mapimage->load(meta->GetImageFilename());
    if (!result)
    {
        if (mapimage)
            delete mapimage;
        
        QMessageBox msg;
        msg.setText(QString("Unable to load map ") + filename);
        msg.setIcon(QMessageBox::Warning);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        mapname = QString("<no map loaded>");
        mapimage = 0;
        meta = 0;
    }
    else
    {
        //meta->SetImageFilename(fullpath);
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
    QStringList files = MapList::Instance()->MapNames(); //maplist.keys();
    QMapSelectionDialog *dialog = new QMapSelectionDialog(files);
    connect(dialog,SIGNAL(selectmap(QString)),this,SLOT(MapSelected(QString)));
    dialog->setModal(true);
#ifdef Q_OS_SYMBIAN
    dialog->showFullScreen();
#else
    dialog->show();
#endif
}

void QMapWidget::WaypointSelected(QString name, double lat, double lon)
{
/*
    QMessageBox msg;
    msg.setText(QString("not implemented"));
    msg.setIcon(QMessageBox::Warning);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
*/
    QMessageBox msg;
    WayPoint *w = new WayPoint(lat,lon);
    w->SetName(name);
    WayPointList::Instance().AddWayPoint(w);
    msg.setText(QString("Waypoint added."));
    msg.setIcon(QMessageBox::Warning);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void QMapWidget::RefpointSelected(QString name, double lat, double lon)
{
    QMessageBox msg;
    RefPoint r(lat,lon,x,y);
    r.SetName(name);
    if (meta->AddRefPoint(r)) //meta->AddRefpoint(lat,lon,x,y))
        msg.setText(QString("Refpoint added."));
    else
        msg.setText(QString("Unable to add refpoint."));

    msg.setIcon(QMessageBox::Warning);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void QMapWidget::SelectPoint()
{
    QWaypointDialog *dialog;

    // Meta data present, but not calibrated then add refpoint
    // Else add waypoint
    if (meta && !meta->IsCalibrated())
    {
        dialog = new QWaypointDialog(QString("Add Refpoint:"),QString("ref"),latitude,longitude);
        connect(dialog,SIGNAL(confirmed(QString,double,double)),this,SLOT(RefpointSelected(QString,double,double)));
    }
    else
    {
        if (mapimage && ((state == StScrolling) || (!IsPositionOnMap())))
        {
            double lat=0, lon=0;
            if ((meta) && (meta->XY2Wgs(x,y,lat,lon)))
                dialog = new QWaypointDialog(QString("Add Waypoint:"),QString("wpt"),lat,lon);
        }
        else
        {
            dialog = new QWaypointDialog(QString("Add Waypoint:"),QString("wpt"),latitude,longitude);
        }
        connect(dialog,SIGNAL(confirmed(QString,double,double)),this,SLOT(WaypointSelected(QString,double,double)));
    }

    dialog->setModal(true);
#ifdef Q_OS_SYMBIAN
    dialog->showFullScreen();
#else
    dialog->show();
#endif
}

void QMapWidget::StartTrack()
{
	if (recordtrack)
	{
	    recordtrack->disconnect(SIGNAL(updated(WayPoint&)));
	    GpxIO::Instance()->WriteTrackFile(*recordtrack);
	    recordtrack = 0;
	}
	else
	{
		QTrackDialog *dialog;
		dialog = new QTrackDialog(QString("Start Track:"),QString("trk"));
		connect(dialog,SIGNAL(confirmed(QString)),this,SLOT(TrackStarted(QString)));
	
		dialog->setModal(true);
	#ifdef Q_OS_SYMBIAN
		dialog->showFullScreen();
	#else
		dialog->show();
	#endif
	}
}

void QMapWidget::TrackStarted(QString n)
{
    LOG( "QMapWidget::TrackStarted()\n"; )
    if (recordtrack) return;
    
	recordtrack = new Track;
	recordtrack->SetName(n);
	TrackList::Instance()->AddTrack(recordtrack);
	ShowTrack(recordtrack);
}

void QMapWidget::ShowTrack(Track* t)
{
    LOG( "QMapWidget::ShowTrack()\n"; )
	tracks.append(t);
    for (int i=0; i < t->Length(); i++)
    	ShowTrackPoint(t->GetItem(i));
    connect(t, SIGNAL(updated(WayPoint&)), this, SLOT(ShowTrackPoint(WayPoint&)));
}

void QMapWidget::ShowTrackPoint(WayPoint& w)
{
    LOG( "QMapWidget::ShowTrackPoint()\n"; )
    if (!mapimage) return;
    if (!meta) return;
    if (!meta->IsPositionOnMap(w.Latitude(),w.Longitude())) return;
    double tx, ty;
    meta->Wgs2XY(w.Latitude(),w.Longitude(),tx,ty);
    
    QPainter painter(mapimage);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawEllipse(tx-2,ty-2,4,4);
}

void QMapWidget::HideTracks()
{
	const Track* t;
	while (tracks.length()>0)
	{
		t = tracks.takeFirst();
		disconnect(t, SIGNAL(updated(WayPoint&)));
	}
	
	if (!mapimage) return;
	if (!meta) return;
	
	delete mapimage;
    if (!mapimage->load(meta->GetImageFilename()))
    {
    	mapimage = 0;
    	meta = 0;
    }
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
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}

void QMapWidget::updatePosition(double lat, double lon)
{
    //LOG( "QMapWidget::updatePosition()\n"; )
    latitude = lat;
    longitude = lon;
    if (recordtrack)
        recordtrack->AddPoint(new WayPoint(lat,lon));
    if (state == StFollowGPS)
        FollowGPS();
    else
        update();
}

void QMapWidget::moveMap(int dx, int dy)
{
    if (!mapimage) return;

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
    if ((event->pos().x() > width()-60) && (event->pos().y() < 60)) emit zoomin();
    else if ((event->pos().x() > width()-60) && (event->pos().y() > height()-60)) emit zoomout();
    else if ((event->pos().x() < 50) && (event->pos().y() < 60)) emit waypoint();
    else if ((event->pos().x() > 60) && (event->pos().x() < 120) && (event->pos().y() < 60)) emit track();
    else if ((event->pos().x() < 60) && (event->pos().y() > height()-60)) emit options();
    else if ((event->pos().x() > 60) && (event->pos().x() < 260) && (event->pos().y() > height()-50)) emit datum();
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

void QMapWidget::paintBackground(QPainter& painter)
{
    double w = width();
    double h = height();
    QRectF source(0, 0, 400, 360);
    QRectF target(-w/2, -h/2, w, h);
    painter.setWindow(-w/2,-h/2,w,h);
    painter.drawImage(target, *bgimage, source);
    painter.setViewport(20,20,w-40,h-40);
}

void QMapWidget::paintMap(QPainter& painter)
{
    if (mapimage)
    {
		double w = width();
		double h = height();
        double z = zoomlevels[zoom];
        QRectF source = QRectF(w*z/-2 + x, h*z/-2 + y, w*z, h*z);
        QRectF target = QRectF(w*z/-2, h*z/-2, w*z, h*z);
        painter.setWindow(-w/2*z,-h/2*z,w*z,h*z);
        painter.drawImage(target, *mapimage, source);
        painter.setWindow(-w/2,-h/2,w,h);
    }
}

bool QMapWidget::IsPositionOnScreen(WayPoint& wpt)
{
    LOG2( "QMapWidget::IsPositionOnScreen()\n"; )
	double px,py;
    double w = width();
    double h = height();
    double z = zoomlevels[zoom];
    LOG2( "QMapWidget::IsPositionOnScreen()  XY: " << x << "," << y << "\n"; )
    LOG2( "QMapWidget::IsPositionOnScreen() WHZ: " << w << "," << h << "," << z << "\n"; )
    
	if (!meta) return false;
	if (!meta->IsPositionOnMap(wpt.Latitude(),wpt.Longitude())) return false;
	if (!meta->Wgs2XY(wpt.Latitude(),wpt.Longitude(),px,py)) return false;
    LOG2( "QMapWidget::IsPositionOnScreen() PXY: " << px << "," << py << "\n"; )
	
	// now x,y are on the map
	if (px > w*z/2 + x) return false;
	if (px < w*z/-2 + x) return false;
	if (py > h*z/2 + y) return false;
	if (py < w*z/-2 + y) return false;
    LOG2( "QMapWidget::IsPositionOnScreen(): Yes!\n"; )
	return true;
}

ScreenPos QMapWidget::PositionOnScreen(WayPoint& wpt)
{
	ScreenPos p;
	double px,py;
    double w = width();
    double h = height();
    double z = zoomlevels[zoom];
	meta->Wgs2XY(wpt.Latitude(),wpt.Longitude(),px,py);
    p.x = (px-x)/z;
    p.y = (py-y)/z;
	return p;
}

void QMapWidget::paintWaypoints(QPainter& painter)
{
	WayPointList& wl = WayPointList::Instance();
	QList<QString> keys = wl.WptNames();
	for (int i=0; i<keys.length(); i++)
	{
        if (IsPositionOnScreen(wl.GetItem(keys[i])))
		{
	        ScreenPos p = PositionOnScreen(wl.GetItem(keys[i]));
	        LOG2( "QMapWidget::paintWaypoints(): " << keys[i].toStdString() << ", " << p.x << "," << p.y << "\n"; )
	        paintDot(painter,p.x,p.y,Qt::blue);
		}
	}
}

void QMapWidget::paintWidgets(QPainter& painter)
{
    QRectF source;
    QRectF target;
    double w = width();
    double h = height();
    source = QRectF(0,0,48,48);
    target = QRectF(w/2-48,h/-2,48,48);
    painter.setViewport(12,12,w-24,h-24);
    painter.drawImage(target, *svgZoomIn, source);
    target = QRectF(w/2-48,h/2-48,48,48);
    painter.drawImage(target, *svgZoomOut, source);
    target = QRectF(w/-2,h/2-48,48,48);
    painter.drawImage(target, *svgOptions, source);
    target = QRectF(w/-2,h/-2,48,48);
    painter.drawImage(target, *svgFlag, source);
    target = QRectF(w/-2+60,h/-2,48,48);
    painter.drawImage(target, *svgHiker, source);
}

void QMapWidget::paintDot(QPainter& painter,int x,int y,QColor c)
{
    double h = height();
    double s = h / 36;
    painter.setPen(c);
    painter.setBrush(c);
    painter.drawEllipse(s/-2+x,s/-2+y,s,s);
}

void QMapWidget::paintBar(QPainter& painter)
{
    double w = width();
    double h = height();
    double s = h / 36;
    QRectF source = QRectF(0,0,300,48);
    QRectF target = QRectF(w/-2,h/2-48,300,48);
    painter.drawImage(target, *svgBar, source);
    char buf[25];
    sprintf(buf,"%s",mapname.toStdString().c_str());
    painter.setFont(QFont("Courier", 168/TEXTDIVIDER));
    QRect r = painter.boundingRect(w/-2+58,h/2-38,260,28, Qt::AlignLeft, buf);
    painter.setPen(QPen(Qt::blue));
    painter.drawText(r, Qt::AlignLeft, buf);

    if ((state == StScrolling) || (!IsPositionOnMap()))
    {
        paintDot(painter,0,0,Qt::red);
		painter.setPen(QPen(Qt::black));
        double lat, lon;
        if ((meta) && (meta->XY2Wgs(x,y,lat,lon)))
            sprintf(buf,"%08.5fN %08.5fE",lat,lon);
        else
            sprintf(buf,"%04.0f,%04.0f",x,y);
    }
    else
    {
        paintDot(painter,0,0,Qt::green);
        painter.setPen(QPen(Qt::blue));
        sprintf(buf,"%08.5fN %08.5fE",latitude,longitude);
    }

    r = painter.boundingRect(w/-2+58,h/2-25,260,28, Qt::AlignLeft, buf);
    painter.drawText(r, Qt::AlignLeft, buf);
}

void QMapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    double w = width();
    double h = height();
    double s = h / 36;

    QPainter painter(this);
    paintBackground(painter);
    paintMap(painter);
    paintWaypoints(painter);
    paintWidgets(painter);
    paintBar(painter);
}
