#include <QPainter>
#include <QFlags>
#include <QIcon>
#include <cmath>
#include <QDir>
#include "ui.h"
#include "qmapwidget.h"
#include "qmapdialog.h"
#include "qwaypointdialog.h"
#include "qtrackdialog.h"
#include "gpxio.h"

#include <QDebug>
#define LOG( a ) qDebug() << a
#define LOG2( a ) 

#ifdef Q_OS_SYMBIAN
#include "eikenv.h"
#define UNSET_SYSTEMAPP { CEikonEnv::Static()->SetSystem( EFalse ); }
#define SET_SYSTEMAPP { CEikonEnv::Static()->SetSystem( ETrue ); }
#else
#define UNSET_SYSTEMAPP
#define SET_SYSTEMAPP
#endif

static double zoomlevels[] = { 0.33, 0.5, 0.66, 1.0, 1.41, 2.0, 3.0 };
const int     zoomneutral = 3;
const int     zoommax = 6;

#ifdef Q_OS_SYMBIAN
#define MAPDIR "/data/qtracker/maps/"
#else
#define MAPDIR "/Users/hurenkam/workspace/qtracker/maps/"
#endif

QMapWidget::QMapWidget(QSettings& s, QWidget *parent)
    : QGaugeWidget(parent)
    , state         (StNoMap)
    , zoom          (s.value("map/zoom",zoomneutral).toInt())
    , x             (0)
    , y             (0)
    , latitude      (0)
    , longitude     (0)
    , meta          (0)
    , mapimage      (0)
	, bgimage       (new QImage(MAPRCDIR "map.svg"))
	, svgZoomIn     (new QImage(MAPRCDIR "zoom-in.svg"))
	, svgZoomOut    (new QImage(MAPRCDIR "zoom-out.svg"))
	, svgOptions    (new QImage(MAPRCDIR "options.svg"))
	, svgFlag       (new QImage(MAPRCDIR "flag.svg"))
	, svgHiker      (new QImage(MAPRCDIR "hiker.svg"))
	, svgBar        (new QImage(MAPRCDIR "statusbar.svg"))
    , svgLocator    (new QImage(MAPRCDIR "locator_red.svg"))
    , onmap			(false)
    , svgWptGreen   (new QImage(MAPRCDIR "wpt_green.svg"))
    , zooming       (0)
    , mapname       ("<no map loaded>")
    , recordtrack   (0)
    , prevpos       (0)
    , ismapdirty    (false)
    , settings(s)
    , devinfo(this)
{
    prevtime = QDateTime::fromTime_t(0);
    connect(this, SIGNAL(drag(int,int)), this, SLOT(moveMap(int,int)));
    connect(this, SIGNAL(singleTap()), this, SLOT(FollowGPS()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(SelectMap()));
    CreateMapList();
    connect(&zoomtimer,SIGNAL(timeout()),this,SLOT(zoomRepeat()));
    zoomtimer.setInterval(150);
    connect(this, SIGNAL(zoomin()), this, SLOT(zoomIn()));
    connect(this, SIGNAL(zoomout()), this, SLOT(zoomOut()));
    connect(this, SIGNAL(options()), this, SLOT(SelectMap()));  // to be menu
    connect(this, SIGNAL(datum()), this, SLOT(SelectMap()));  // to be menu
    connect(this, SIGNAL(waypoint()), this, SLOT(SelectWayPoint()));
    connect(this, SIGNAL(track()), this, SLOT(StartTrack()));
    connect(TrackList::Instance(),SIGNAL(added(Track*)),this,SLOT(ShowTrack(Track*)));
	connect(TrackList::Instance(),SIGNAL(removed(QString)),this,SLOT(HideTrack(QString)));
    //connect(WayPointList::Instance(),SIGNAL(added(const WayPoint&)),this,SLOT(ShowWayPoint(const WayPoint&)));
    //connect(WayPointList::Instance(),SIGNAL(deleted(const QString&)),this,SLOT(HideWayPoint(const QString&)));
	connect(&devinfo, SIGNAL(batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)),this,SLOT(batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)));
	
	if (settings.contains("map/name"))
	{
        if (LoadMap(settings.value("map/name").toString()) && (settings.contains("map/x") && (settings.contains("map/y"))))
		{
			x = settings.value("map/x").toInt();
			y = settings.value("map/y").toInt();
			state = settings.value("map/state",StScrolling).toInt();
		}
	}
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

void QMapWidget::batteryStatusChanged(QSystemDeviceInfo::BatteryStatus status)
{
    LOG( "QMapWidget::batteryStatusChanged()"; )
	if (status != QSystemDeviceInfo::BatteryCritical) return;
    if (!recordtrack) return;

    GpxIO::Instance()->WriteTrackFile(*recordtrack);
    LOG( "QMapWidget::batteryStatusChanged(): Battery Critical, track saved."; )

	QMessageBox msg;
    msg.setText(QString("Battery Critical: track saved."));
    msg.setIcon(QMessageBox::Warning);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void QMapWidget::CreateMapList()
{
}

bool QMapWidget::SelectBestMapForCurrentPosition()
{
    LOG( "QMapWidget::SelectBestMapForCurrentPosition()"; )

    QStringList found;
    FindMapsForCurrentPosition(found);
    if (found.size() > 0)
    {
        LOG( "QMapWidget::SelectBestMapForCurrentPosition(): " << found[0]; )
        // for now select first entry
        int index = 0;
        MapMetaData m = MapList::Instance().GetItem(found[0]);
        int lon2x = m.Lon2x();
        for (int i=1; i<found.size(); ++i)
        {
        	m = MapList::Instance().GetItem(found[i]);
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
    LOG( "QMapWidget::FindMapsForCurrentPosition()"; )

    QStringList keys = MapList::Instance().MapNames();
    for (int i=0; i<keys.size(); ++i)
    {
        if (MapList::Instance().GetItem(keys[i]).IsPositionOnMap(latitude,longitude))
        {
            LOG( "QMapWidget::FindMapsForCurrentPosition(): " << keys[i]; )
            found.append(keys[i]);
        }
    }
}

bool QMapWidget::LoadMap(QString filename)
{
    LOG( "QMapWidget::LoadMap(): " << filename; )
    //QString fullpath = GetDrive() + QString(MAPDIR) + filename + QString(".jpg");

    if (mapimage)
        delete mapimage;

    mapimage = new QImage();
    meta = &MapList::Instance().GetItem(filename);
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
        ismapdirty = false;
    }
    else
    {
        //meta->SetImageFilename(fullpath);
        meta->SetSize(mapimage->width(),mapimage->height());
        meta->Calibrate();
        x = mapimage->width()/2;
        y = mapimage->height()/2;
        mapname = filename;
        ismapdirty = true;
        
        settings.setValue("map/name",mapname);
        settings.setValue("map/x",x);
        settings.setValue("map/y",y);
    }
    update();
    return result;
}

void QMapWidget::MapSelected(QString map)
{
    LOG( "QMapWidget::MapSelected()"; )
    if (LoadMap(map))
        state = StScrolling;
    else
        state = StNoMap;
    settings.setValue("map/state",state);
}

void QMapWidget::RefpointSelected(QString name, double lat, double lon)
{
    QMessageBox msg;
    //RefPoint r(lat,lon,x,y);
    RefPoint r(x,y,lat,lon);
    r.SetName(name);
    if (meta->AddRefPoint(r)) //meta->AddRefpoint(lat,lon,x,y))
    {
        msg.setText(QString("Refpoint added."));
        msg.setIcon(QMessageBox::Information);
    }
    else
    {
        msg.setText(QString("Unable to add refpoint."));
        msg.setIcon(QMessageBox::Warning);
    }
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void QMapWidget::SelectWayPoint()
{
	QWayPointTabsDialog *dialog;

	WayPoint wpt = WayPoint(latitude,longitude,altitude,GeoTime(),"<new>");
	double lat=0, lon=0;
	if ( mapimage && ((state == StScrolling) || (!IsPositionOnMap()))
		 && meta && (meta->XY2Wgs(x,y,lat,lon)) )
	{
		wpt.SetLatitude(lat);
		wpt.SetLongitude(lon);
		wpt.SetElevation(0);
	}
	dialog = new QWayPointTabsDialog(wpt,this);
    dialog->setModal(true);
    dialog->show();
}

void QMapWidget::StartTrack() // name doesn't cover the load, should be renamed
{
	QTrackTabsDialog *dialog;
	dialog = new QTrackTabsDialog(recordtrack);
	
	connect(dialog,SIGNAL(newtrack(QString,int,int)),this,SLOT(TrackStarted(QString,int,int)));
	connect(dialog,SIGNAL(updatetrack(QString,int,int)),this,SLOT(TrackUpdated(QString,int,int)));
	connect(dialog,SIGNAL(stoptrack(QString)),this,SLOT(TrackStopped(QString)));
	connect(dialog,SIGNAL(deletetrack(const QString&)),this,SLOT(TrackDeleted(const QString&)));
	connect(dialog,SIGNAL(showtrack(const QString&)),this,SLOT(TrackLoad(const QString&)));
	connect(dialog,SIGNAL(hidetrack(const QString&)),this,SLOT(TrackUnload(const QString&)));

	dialog->setModal(true);
	dialog->show();
}

void QMapWidget::TrackStarted(QString n, int t, int d)
{
    LOG( "QMapWidget::TrackStarted()"; )
    if (recordtrack) return;
    
	recordtrack = new Track;
	recordtrack->SetName(n);
	updatetime = t;
	updatedistance = d;
	TrackList::Instance()->AddTrack(recordtrack);
	//ShowTrack(recordtrack);
	
	SET_SYSTEMAPP
}

void QMapWidget::TrackUpdated(QString n, int t, int d)
{
    LOG( "QMapWidget::TrackUpdated()"; )
	updatetime = t;
	updatedistance = d;
}

void QMapWidget::TrackStopped(const QString& name)
{
    LOG( "QMapWidget::TrackStopped()"; )
	if (!recordtrack) return;
	
	recordtrack->disconnect(SIGNAL(updated(WayPoint&)));
	GpxIO::Instance()->WriteTrackFile(*recordtrack);
	recordtrack = 0;
	prevpos = 0;
	prevtime = QDateTime::fromTime_t(0);

	UNSET_SYSTEMAPP
}

void QMapWidget::TrackDeleted(const QString& name)
{
    LOG( "QMapWidget::TrackDeleted()"; )
    		
	QString filename = name + ".gpx";
    QDir dir = QDir(TRACKDIR);

	if ( !dir.remove(filename) )
	{	
		QMessageBox msg;
		msg.setText(QString("Unable to delete file."));
		msg.setIcon(QMessageBox::Warning);
		msg.setStandardButtons(QMessageBox::Ok);
		msg.exec();
	}
}

void QMapWidget::paintTrack(Track* t)
{
	for (int i=0; i < t->Length(); i++)
		ShowTrackPoint(t->GetItem(i));
}

void QMapWidget::ShowTrack(Track* t)
{
    LOG( "QMapWidget::ShowTrack()"; )
	//tracks[t->Name()]=t;
    paintTrack(t);
    connect(t, SIGNAL(updated(WayPoint&)), this, SLOT(ShowTrackPoint(WayPoint&)));
}

void QMapWidget::HideTrack(QString name)
{
    LOG( "QMapWidget::HideTrack()"; )

	if (!ismapdirty)
	{
	    LOG( "QMapWidget::HideTrack() map not dirty so reload"; )
		delete mapimage;
		mapimage = new QImage();
		meta = &MapList::Instance().GetItem(mapname);
		bool result = mapimage->load(meta->GetImageFilename());
		if (!result)
		{
			if (mapimage)
				delete mapimage;
			
			QMessageBox msg;
			msg.setText(QString("Unable to load map ") + mapname);
			msg.setIcon(QMessageBox::Warning);
			msg.setStandardButtons(QMessageBox::Ok);
			msg.exec();
			mapname = QString("<no map loaded>");
			mapimage = 0;
			meta = 0;
		}
		ismapdirty = true;
	}
}

void QMapWidget::ShowTrackPoint(WayPoint& w)
{
    LOG2( "QMapWidget::ShowTrackPoint()"; )
    if (!mapimage) return;
    if (!meta) return;
    if (!meta->IsPositionOnMap(w.Latitude(),w.Longitude())) return;
    double tx, ty;
    meta->Wgs2XY(w.Latitude(),w.Longitude(),tx,ty);
    
    QPainter painter(mapimage);
    painter.setPen(Qt::yellow);
    painter.setBrush(Qt::yellow);
    painter.drawEllipse(tx-2,ty-2,4,4);
}

void QMapWidget::TrackLoad(const QString& name)
{
    LOG( "QMapWidget::TrackLoad()" << name; )
		
    QString filename = TRACKDIR + name + ".gpx";
    GpxIO::Instance()->ImportGpxFile(filename);
}

void QMapWidget::TrackUnload(const QString& name)
{
    LOG( "QMapWidget::TrackUnload()" << name; )
    TrackList::Instance()->RemoveTrack(name);
}

void QMapWidget::SelectMap()
{
	if (!meta) return;
	
	RefPoint r = RefPoint(x,y,latitude,longitude);
	QMapTabsDialog *dialog = new QMapTabsDialog(this,meta,&r);
    connect(dialog,SIGNAL(loadmap(QString)),this,SLOT(MapSelected(QString)));
	dialog->setModal(true);
	dialog->show();
}

void QMapWidget::updatePosition(const QGeoCoordinate& pos)
{
    //LOG( "QMapWidget::updatePosition()"; )
    latitude = pos.latitude();
    longitude = pos.longitude();
    altitude = pos.altitude();
    
	QDateTime curtime = QDateTime::currentDateTime().toUTC();
	QString timestamp = curtime.toString("yyyy-MM-ddThh:mm:ssZ");
	WayPoint* curpos = new WayPoint(latitude,longitude,altitude,timestamp);
	
	int deltatime = 0;
	double deltadistance = 0;
	
    if (recordtrack)
	{
    	deltatime = prevtime.secsTo(curtime);
        if (prevpos != 0)
        	deltadistance = prevpos->distance(curpos);
		
		if ((updatetime==0) && (updatedistance==0))
		{
            recordtrack->AddPoint(curpos);
            curpos = 0;
		}
		else if ((updatetime!=0) && (updatetime < deltatime))
		{
            prevtime = curtime;
            recordtrack->AddPoint(curpos);
            curpos = 0;
        }
		else if ((updatedistance!=0) && ((prevpos==0) || (updatedistance < deltadistance)))
		{
        	prevpos = curpos;
            recordtrack->AddPoint(curpos);
            curpos = 0;
        }
	}
    if (state == StFollowGPS)
        FollowGPS();
    else
        update();
    
    if (curpos)
    	delete curpos;
}

void QMapWidget::moveMap(int dx, int dy)
{
    if (!mapimage) return;

    x -= dx * zoomlevels[zoom];
    y -= dy * zoomlevels[zoom];
    if (state == StFollowGPS)
        state = StScrolling;
    
    svgLocator->load(MAPRCDIR "locator_red.svg");
    onmap = false;
    settings.setValue("map/x",x);
    settings.setValue("map/y",y);
    settings.setValue("map/state",state);
    update();
}

void QMapWidget::FollowGPS()
{
    LOG( "QMapWidget::FollowGPS()"; )

    if (IsPositionOnMap())
    {   // OnMap
    	if (!onmap)
    	    svgLocator->load(MAPRCDIR "locator_green.svg");
    	onmap = true;
        SetCursorToCurrentPosition();
        state = StFollowGPS;
        settings.setValue("map/x",x);
        settings.setValue("map/y",y);
        settings.setValue("map/state",state);
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
    settings.setValue("map/zoom",zoom);
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
	if (ismapdirty)
    {
	    LOG( "QMapWidget::paintMap() map dirty so repaint tracks."; )
		QStringList keys=TrackList::Instance()->Keys();
		for (int i=0; i<keys.length(); i++)
			paintTrack(&TrackList::Instance()->GetItem(keys[i]));
		ismapdirty = false;
    }
	
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

bool QMapWidget::IsPositionOnScreen(const WayPoint& wpt)
{
    LOG( "QMapWidget::IsPositionOnScreen()"; )
	double px,py;
    double w = width();
    double h = height();
    double z = zoomlevels[zoom];
    LOG( "QMapWidget::IsPositionOnScreen()  XY: " << x << "," << y; )
    LOG( "QMapWidget::IsPositionOnScreen() WHZ: " << w << "," << h << "," << z; )
    
	if (!meta) return false;
	if (!meta->IsPositionOnMap(wpt.Latitude(),wpt.Longitude())) return false;
	if (!meta->Wgs2XY(wpt.Latitude(),wpt.Longitude(),px,py)) return false;
    LOG( "QMapWidget::IsPositionOnScreen() PXY: " << px << "," << py; )
	
	// now x,y are on the map
	if (px > w*z/2 + x) return false;
	if (px < w*z/-2 + x) return false;
	if (py > h*z/2 + y) return false;
	if (py < w*z/-2 + y) return false;
    LOG( "QMapWidget::IsPositionOnScreen(): Yes!"; )
	return true;
}

ScreenPos QMapWidget::PositionOnScreen(const WayPoint& wpt)
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
	QList<QString> keys = wl.VisibleKeys();
	for (int i=0; i<keys.length(); i++)
	{
        if (IsPositionOnScreen(wl.GetItem(keys[i])))
		{
	        ScreenPos p = PositionOnScreen(wl.GetItem(keys[i]));
	        LOG( "QMapWidget::paintWaypoints(): " << keys[i] << ", " << p.x << "," << p.y; )
	        //paintDot(painter,p.x,p.y,Qt::blue);

	        QRectF source = QRectF(0,0,48,48);
	        QRectF target = QRectF(p.x-7,p.y-60,64,64);
	        painter.drawImage(target, *svgWptGreen, source);
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
    target = QRectF(-16.0,-16.0,32,32);
    painter.drawImage(target, *svgLocator, source);
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
        //paintDot(painter,0,0,Qt::red);
		painter.setPen(QPen(Qt::black));
        double lat, lon;
        if ((meta) && (meta->XY2Wgs(x,y,lat,lon)))
            sprintf(buf,"%08.5fN %08.5fE",lat,lon);
        else
            sprintf(buf,"%04.0f,%04.0f",x,y);
    }
    else
    {
        //paintDot(painter,0,0,Qt::green);
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
