#include "mapview.h"
#include <QImageReader>
#include <QPainter>
#include <QFileInfo>
#include <QDir>
#include <math.h>

#include "qmldatabase.h"
#include "qmlwaypoint.h"
#include <QGeoBoundingBox>
#include <QGeoCoordinate>

using namespace QtMobility;

//#define ENABLE_DEBUG
#include "helpers.h"
#define ENTER1(a) ENTER(a)
#define ENTER2(a)
#define ENTER3(a)
#define EXIT1(a) EXIT(a)
#define EXIT2(a)
#define EXIT3(a)
#define LOG1(a) LOG(a)
#define LOG2(a)
#define LOG3(a)

const int tilesize = 512;
const int requestdelay = 100;

// maxtiles must be >= (640/(512/maxfactor) + 1) * (360/(512/maxfactor) + 1)
// currently maxfactor == 3.0 (see mapview.qml)
const int maxtiles = 24;

static bool paintdebug = false;
//const int minzoom = 0;
//const int maxzoom = 4;

Area::Area(qmlWaypoint* topleft, qmlWaypoint* bottomright)
    : QGeoBoundingBox()
{
    setTopLeft(QGeoCoordinate(topleft->latitude(),topleft->longitude(),topleft->altitude()));
    setBottomRight(QGeoCoordinate(bottomright->latitude(),bottomright->longitude(),bottomright->altitude()));
}

Area::Area(const qmlWaypoint& topleft, const qmlWaypoint& bottomright)
    : QGeoBoundingBox()
{
    setTopLeft(QGeoCoordinate(topleft.latitude(),topleft.longitude(),topleft.altitude()));
    setBottomRight(QGeoCoordinate(bottomright.latitude(),bottomright.longitude(),bottomright.altitude()));
}

MapView::MapView(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
    , _filename("")
    , _trackid(-1)
    , _mapid(-1)
    , _map(0)
    //, area(0)
{
    ENTER1("")
    _scale = 0.5;
    QPixmap t(tilesize,tilesize);
    t.fill(Qt::lightGray);
    empty = t.toImage();
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    _filename = "";
    mythread = new TileLoader();
    connect(this,     SIGNAL(loadTile(QPoint,QUrl)),     mythread, SLOT(onTileRequest(QPoint,QUrl)));
    connect(mythread, SIGNAL(tileLoaded(QPoint,QImage)), this,     SLOT(onTileLoaded(QPoint,QImage)));
    connect(mythread, SIGNAL(invalidate()),              this,     SLOT(onInvalidate()));
    mythread->start();
    EXIT1("")
}

void MapView::setFilename(QUrl u)
{
    ENTER1(u)
    _filename = u;
    reader.setFileName(u.toLocalFile());
    _filesize = reader.size();
    emit filesizeChanged();
    _mapx = 0;
    _mapy = 0;
    tiles.clear();
    update();
    emit filenameChanged();
    EXIT1("")
}

void MapView::setMapid(int id)
{
    ENTER1("")
    if (id < 0) return;

    _mapid = id;
    emit mapidChanged();
    if (_map) delete _map;
    _map = new qmlMap(_mapid);
    if (_map)
    {
        emit nameChanged();
        //setFilename(_map->url());
        setFilename(QUrl(_map->filename()));
        _map->calibrate();
        emit calibrationChanged();
    }
    EXIT1("")
}

QRect MapView::viewTiles()
{
    ENTER2("")
    double w = width();
    double h = height();
    double x = ((double)_mapx) * _scale;
    double y = ((double)_mapy) * _scale;
    double t = ((double)tilesize) * _scale;
    //qDebug() << "x,y,w,h,t,_scale: " << x << y << w << h << t << _scale;

    int viewleft =   (int)((x-w/2.0) / t);
    int viewright =  (int)((x+w/2.0) / t);
    int viewtop =    (int)((y-w/2.0) / t);
    int viewbottom = (int)((y+h/2.0) / t);

    //qDebug() << "viewArea" << viewleft << viewtop << viewright << viewbottom << t;
    return QRect(viewleft,viewtop,(viewright-viewleft+1),(viewbottom-viewtop+1));
}

QRect MapView::mapTiles()
{
    ENTER2("")
    int w = _filesize.width()/tilesize;
    if (_filesize.width()%tilesize) w++;
    int h = _filesize.height()/tilesize;
    if (_filesize.height()%tilesize) h++;
    return QRect(QPoint(0,0),QSize(w,h));
}

QRect MapView::sourceArea(const QPoint& p)
{
    ENTER3("")
    return QRect(QPoint(0,0),QSize(tilesize,tilesize));
}

QRect MapView::targetArea(const QPoint& p)
{
    ENTER3("")
    int w = width();
    int h = height();
    int dx = p.x()*tilesize*_scale-_mapx*_scale+w/2;
    int dy = p.y()*tilesize*_scale-_mapy*_scale+h/2;

    return QRect(QPoint(dx,dy),QSize(tilesize*_scale,tilesize*_scale));
}

qmlWaypoint* MapView::MapXY2Waypoint(const QPoint& p)
{
    double lon=0;
    double lat=0;
    if (_map && _map->isCalibrated())
    {
        lon = _map->mapx2lon(p.x());
        lat = _map->mapy2lat(p.y());
    }
/*
    else
    {
        lon = area->topLeft().longitude() + p.x()/_filesize.width()  * (area->bottomRight().longitude() - area->topLeft().longitude());
        lat = area->topLeft().latitude()  + p.y()/_filesize.height() * (area->bottomRight().latitude()  - area->topLeft().latitude());
    }
*/
    return new qmlWaypoint(lat,lon);
}

QPoint MapView::Waypoint2MapXY(const qmlWaypoint& w)
{
    int x=0;
    int y=0;
    if (_map && _map->isCalibrated())
    {
        x = _map->lon2mapx(w.longitude());
        y = _map->lat2mapy(w.latitude());
    }
/*
    else
    {
        x = (w.longitude()-area->topLeft().longitude()) * _filesize.width()  / (area->bottomRight().longitude() - area->topLeft().longitude());
        y = (w.latitude() -area->topLeft().latitude())  * _filesize.height() / (area->bottomRight().latitude()  - area->topLeft().latitude());
    }
*/
    return QPoint(x,y);
}

QPoint MapView::MapXY2ViewXY(const QPoint& p)
{
    int w = width();
    int h = height();
    double x = (p.x()-_mapx)*_scale+w/2;
    double y = (p.y()-_mapy)*_scale+h/2;
    return QPoint(x,y);
}

Area MapView::geoArea(const QPoint& p)
{
    ENTER1(p)
/*
    QPoint topleft(     p.x()   *tilesize, p.y()   *tilesize);
    QPoint bottomright((p.x()+1)*tilesize,(p.y()+1)*tilesize);
    Area result(MapXY2Waypoint(topleft),MapXY2Waypoint(bottomright));

    LOG("MapView::geoArea() " << result.topLeft().latitude() << ", " <<  result.topLeft().longitude() << ", " << result.bottomRight().latitude() << ", " << result.bottomRight().longitude() )
*/
    QPoint topleft(     p.x()   *tilesize-5, p.y()   *tilesize-5);
    QPoint bottomright((p.x()+1)*tilesize+5,(p.y()+1)*tilesize+5);
    qmlWaypoint *tl = MapXY2Waypoint(topleft);
    qmlWaypoint *br = MapXY2Waypoint(bottomright);
    Area result(tl,br);

    LOG1("MapView::geoArea() " << result.topLeft().latitude() << ", " <<  result.topLeft().longitude() << ", " << result.bottomRight().latitude() << ", " << result.bottomRight().longitude() )
    delete tl;
    delete br;
    return result;
}

void MapView::renderTile(QPainter *painter, QPoint& p)
{
    ENTER3("")
    QRect s = sourceArea(p);
    QRect t = targetArea(p);
    painter->drawImage(t,tiles[p],s);
    EXIT3("")
}

void MapView::renderEmpty(QPainter *painter, QPoint& p)
{
    ENTER3("")
    QRect s = sourceArea(p);
    QRect t = targetArea(p);
    painter->drawImage(t,empty,s);
    EXIT3("")
}

void MapView::renderWaypoints(QPainter *painter, QPoint& p)
{
    if (!_map || !_map->isCalibrated()) return;
/*
    ENTER2(p)
    QRect s(sourceArea(p));
    Area a(geoArea(p));
    WaypointList w = Database::Instance().Waypoints(a);
    LOG("render area (" << a.topLeft() << " " << a.bottomRight() << ") contains " << w.count() << " waypoints")

    QPen pen;
    pen.setWidth(5);
    pen.setColor(Qt::blue);
    painter->setPen(pen);
    for (int i=0; i<w.count(); i++)
    {
        QPoint mappt =  Waypoint2MapXY(w[i]);
        QPoint viewpt = MapXY2ViewXY(mappt);
        if ( (viewpt.x()>=0) && (viewpt.x()<=width()) && (viewpt.y()>=0) && (viewpt.y()<=height()))
        {
            LOG("render waypoint: " << w[i].getName() << w[i] << " " << mappt << " " << viewpt)
            painter->drawEllipse(viewpt,5,5);
        }
    }
    //painter->drawEllipse(QPoint(100,100),5,5);
*/
    EXIT2("")
}

void MapView::renderWaypoints(QPainter *painter)
{
    if (!_map) return;
    if (!_map->isCalibrated()) return;

    ENTER1("")
/*
    Area a(_map->area());
    WaypointList w = Database::Instance().Waypoints(a);
    LOG("render area (" << a.topLeft() << " " << a.bottomRight() << ") contains " << w.count() << " waypoints")

    QPen pen;
    pen.setWidth(5);
    pen.setColor(Qt::blue);
    painter->setPen(pen);
    for (int i=0; i<w.count(); i++)
    {
        QPoint mappt =  Waypoint2MapXY(w[i]);
        QPoint viewpt = MapXY2ViewXY(mappt);
        LOG("render waypoint: " << w[i].getName() << w[i] << " " << mappt << " " << viewpt)
        if ( (viewpt.x()>=0) && (viewpt.x()<=width()) && (viewpt.y()>=0) && (viewpt.y()<=height()))
        {
            painter->drawEllipse(viewpt,5,5);
        }
    }
*/
/* These items were used to check the conversion routines.
    pen.setColor(Qt::red);
    painter->setPen(pen);
    painter->drawEllipse(MapXY2ViewXY(QPoint(0,0)),10,10);
    painter->drawEllipse(MapXY2ViewXY(QPoint(0,_filesize.height())),10,10);
    painter->drawEllipse(MapXY2ViewXY(QPoint(_filesize.width(),0)),10,10);
    painter->drawEllipse(MapXY2ViewXY(QPoint(_filesize.width(),_filesize.height())),10,10);

    pen.setColor(Qt::green);
    painter->setPen(pen);
    painter->drawEllipse ( MapXY2ViewXY ( Waypoint2MapXY ( Waypoint(51.4810253, 5.4591793) ) ), 10, 10 );
    painter->drawEllipse ( MapXY2ViewXY ( Waypoint2MapXY ( Waypoint(51.4810253, 5.6025814) ) ), 10, 10 );
    painter->drawEllipse ( MapXY2ViewXY ( Waypoint2MapXY ( Waypoint(51.3639954, 5.4591793) ) ), 10, 10 );
    painter->drawEllipse ( MapXY2ViewXY ( Waypoint2MapXY ( Waypoint(51.3639954, 5.6025814) ) ), 10, 10 );
*/
    EXIT1("")
}

void MapView::invalidate()
{
    ENTER1("")
    QRect r = viewTiles();
    QHash<QPoint,QImage>::iterator i;
    //qDebug() << "invalidate(); valid rect: " << r;
    for (i=tiles.begin(); i!=tiles.end(); ++i)
    {
        if (!r.contains(i.key()))
        {
            //qDebug() << "remove key       " << i.key();
            discardTile(i.key());
            break;
        }        
        else
        {
            //qDebug() << "not removing key " << i.key();
        }
    }
    EXIT1("")
}

void MapView::clearcache()
{
    ENTER1("")
    QRect r = viewTiles();
    QHash<QPoint,QImage>::iterator i;
    //qDebug() << "invalidate(); valid rect: " << r;
    for (i=tiles.begin(); i!=tiles.end(); ++i)
    {
        discardTile(i.key());
    }
    EXIT1("")
}

void MapView::discardTile(const QPoint& p)
{
    ENTER3("")
    if (tiles.contains(p))
        tiles.remove(p);
    else
        qDebug() << "tried to discard unknown tile" << p;
}

void MapView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ENTER1("")
    if (_filename.toLocalFile()=="") return;

    QRect r = mapTiles().intersected(viewTiles());
    for (int x = r.x(); x<(r.x()+r.width()); x++)
    {
        for (int y=r.y(); y<(r.y()+r.height()); y++)
        {
            QPoint p(x,y);
            if (tiles.contains(p))
            {
                renderTile(painter,p);
            }
            else
            {
                emit loadTile(p,_filename);
                renderEmpty(painter,p);
            }
            renderWaypoints(painter,p);
        }
    }
    //renderWaypoints(painter);
    paintdebug = false;
}

void MapView::setMapX(int v)
{
    ENTER3("")
    _mapx = v; update();
}

void MapView::setMapY(int v)
{
    ENTER3("")
    _mapy = v; update();
}

void MapView::setScale(double v)
{
    ENTER2("")
    _scale = v; update();
}

void MapView::setArea(QVariantMap a)
{
    ENTER1(a)

    if (area) delete area;
    area = new Area(qmlWaypoint(a["top"].toDouble(),a["left"].toDouble()),qmlWaypoint(a["bottom"].toDouble(),a["right"].toDouble()));
}

void MapView::setTrackid(int id)
{
    _trackid = id;
    emit trackidChanged();
}

void MapView::onTileLoaded(const QPoint& p, QImage i)
{
    ENTER3("")
    tiles[p]=i;
    if (tiles.count()>maxtiles) invalidate();
    update();
}

bool operator==(const Request& a, const Request& b)
{
    return ((a.point == b.point) && (a.url == b.url));
}

TileLoader::TileLoader(QObject *parent) : QThread(parent)
{
    connect(this, SIGNAL(reload(QPoint,QUrl)), this, SLOT(onTileRequest(QPoint,QUrl)));
}

void TileLoader::onTileRequest(const QPoint& p, const QUrl& u)
{
    Request r(p,u);
    if (requests.contains(r))
        requests.removeAll(r);

    requests.append(r);
}

void TileLoader::loadTile(const QPoint& p, const QUrl& u)
{
    QString s = u.toLocalFile();
    if (s == "") return;
    if (p.x()<0) return;
    if (p.y()<0) return;

    qDebug() << "loading tile" << p << "for file" << u;

    QPoint pi(p.x()*tilesize,p.y()*tilesize);
    QSize  size(tilesize,tilesize);
    QRect  r(pi,size);
    QImage image;
    QImageReader reader;

    QFileInfo info(s);
    QString cachedir=info.path() + "/" + info.baseName();
    QString sc=QString("%1/%3-%4.jpg")
            .arg(cachedir)
            .arg(p.x())
            .arg(p.y());
    if (QDir().exists(sc))
    {
        //qDebug() << "found cached file" << sc;
        image.load(sc);
    }
    else
    {
        //qDebug() << "creating cachefile" << sc;
        if (!(QDir().exists(cachedir)))
            QDir().mkdir(cachedir);

        reader.setFileName(s);
        reader.setClipRect(r);
        image = reader.read();
        if (!image.isNull())
            image.save(sc);
    }

    if (image.isNull())
    {
        emit invalidate();
        emit reload(p,u);
        onTileRequest(p,u);
    }
    else
    {
        emit tileLoaded(p,image);
    }
}

void TileLoader::run()
{
    //exec();
    Request r;
    while (true)
    {
        if (!requests.isEmpty())
        {
            r = requests.last();
            requests.removeLast();
            loadTile(r.point,r.url);
        }
        msleep(requestdelay);
    }
}

uint qHash(const QPoint& p)
{
    return p.x() * 17^p.y();
}
