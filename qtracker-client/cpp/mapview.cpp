#include "mapview.h"
#include <QImageReader>
#include <QPainter>
#include <QFileInfo>
#include <QDir>
#include <math.h>

//#define ENABLE_DEBUG
#include "helpers.h"

const int tilesize = 512;
const int requestdelay = 100;

// maxtiles must be >= (640/(512/maxfactor) + 1) * (360/(512/maxfactor) + 1)
// currently maxfactor == 3.0 (see mapview.qml)
const int maxtiles = 24;

static bool paintdebug = false;
//const int minzoom = 0;
//const int maxzoom = 4;

MapView::MapView(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    ENTER("")
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
    EXIT("")
}

void MapView::setFilename(QUrl u)
{
    ENTER("")
    _filename = u;
    reader.setFileName(u.toLocalFile());
    _filesize = reader.size();
    emit filesizeChanged();
    _mapx = 0;
    _mapy = 0;
    tiles.clear();
    update();
    EXIT("")
}

QRect MapView::viewTiles()
{
    ENTER("")
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
    ENTER("")
    int w = _filesize.width()/tilesize;
    if (_filesize.width()%tilesize) w++;
    int h = _filesize.height()/tilesize;
    if (_filesize.height()%tilesize) h++;
    return QRect(QPoint(0,0),QSize(w,h));
}

QRect MapView::sourceArea(const QPoint& p)
{
    ENTER("")
    return QRect(QPoint(0,0),QSize(tilesize,tilesize));
}

QRect MapView::targetArea(const QPoint& p)
{
    ENTER("")
    int w = width();
    int h = height();
    int dx = p.x()*tilesize*_scale-_mapx*_scale+w/2;
    int dy = p.y()*tilesize*_scale-_mapy*_scale+h/2;

    return QRect(QPoint(dx,dy),QSize(tilesize*_scale,tilesize*_scale));
}

void MapView::renderTile(QPainter *painter, QPoint& p)
{
    ENTER("")
    QRect s = sourceArea(p);
    QRect t = targetArea(p);
    painter->drawImage(t,tiles[p],s);
    EXIT("")
}

void MapView::renderEmpty(QPainter *painter, QPoint& p)
{
    ENTER("")
    QRect s = sourceArea(p);
    QRect t = targetArea(p);
    painter->drawImage(t,empty,s);
    EXIT("")
}

void MapView::invalidate()
{
    ENTER("")
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
    EXIT("")
}

void MapView::clearcache()
{
    ENTER("")
    QRect r = viewTiles();
    QHash<QPoint,QImage>::iterator i;
    //qDebug() << "invalidate(); valid rect: " << r;
    for (i=tiles.begin(); i!=tiles.end(); ++i)
    {
        discardTile(i.key());
    }
    EXIT("")
}

void MapView::discardTile(const QPoint& p)
{
    ENTER("")
    if (tiles.contains(p))
        tiles.remove(p);
    else
        qDebug() << "tried to discard unknown tile" << p;
}

void MapView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ENTER("")
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
        }
    }
    paintdebug = false;
}

void MapView::setMapX(int v)
{
    ENTER("")
    _mapx = v; update();
}

void MapView::setMapY(int v)
{
    ENTER("")
    _mapy = v; update();
}

void MapView::setScale(double v)
{
    ENTER("")
    _scale = v; update();
}

void MapView::setTrackid(int id)
{
    _trackid = id;
    emit trackidChanged();
}

void MapView::onTileLoaded(const QPoint& p, QImage i)
{
    ENTER("")
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
