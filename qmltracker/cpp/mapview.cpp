#include "mapview.h"
#include <QImageReader>
#include <QPainter>
#include <math.h>

MapView::MapView(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    QPixmap t(256,256);
    t.fill(Qt::lightGray);
    empty = t.toImage();
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    _filename = "";
    mythread = new TileLoader();
    connect(this,     SIGNAL(loadTile(QPoint,QUrl)),     mythread, SLOT(onTileRequest(QPoint,QUrl)));
    connect(mythread, SIGNAL(tileLoaded(QPoint,QImage)), this,     SLOT(onTileLoaded(QPoint,QImage)));
    mythread->start();
}

static bool paintdebug = false;

void MapView::setFilename(QUrl u)
{
    _filename = u;
    reader.setFileName(u.toLocalFile());
    _filesize = reader.size();

    int x = _filesize.width()/256;
    if (_filesize.width()%256) x++;
    int y = _filesize.height()/256;
    if (_filesize.height()%256) y++;
    _tilesize = QSize(x,y);

    _mapx = _filesize.width()/2;
    _mapy = _filesize.height()/2;
    tiles.clear();
    update();
}

void MapView::renderTile(QPainter *painter, QPoint& p)
{
    int w = width();
    int h = height();
    int dx = p.x()*256-_mapx+w/2;
    int dy = p.y()*256-_mapy+h/2;
    if ((dx > w) || (dy > h)) return;

    int sx = 0;
    int sw = 256;
    int sy = 0;
    int sh = 256;
    if (dx < 0)
    {
        sw += dx;
        sx -= dx;
        dx = 0;
    }
    if (dy < 0)
    {
        sh += dy;
        sy -= dy;
        dy = 0;
    }

    painter->drawImage(dx,dy,tiles[p],sx,sy,sw,sh);
}

void MapView::renderEmpty(QPainter *painter, QPoint& p)
{
    int w = width();
    int h = height();
    int dx = p.x()*256-_mapx+w/2;
    int dy = p.y()*256-_mapy+h/2;
    if ((dx > w) || (dy > h)) return;

    int sx = 0;
    int sw = 256;
    int sy = 0;
    int sh = 256;
    if (dx < 0)
    {
        sw += dx;
        sx -= dx;
        dx = 0;
    }
    if (dy < 0)
    {
        sh += dy;
        sy -= dy;
        dy = 0;
    }

    painter->drawImage(dx,dy,empty,sx,sy,sw,sh);
}
/*
bool MapView::loadTile(QPoint& p)
{
    QPoint pi(p.x()*256,p.y()*256);
    QSize  size(256,256);
    QRect  r(pi,size);

    reader.setFileName(_filename.toLocalFile());
    reader.setClipRect(r);
    tiles[p] = reader.read();
    if (tiles[p].isNull())
    {
        if (paintdebug) qDebug() << reader.errorString();
        discardTile(p);
        return false;
    }
    return true;
}
*/
void MapView::invalidate()
{
    if (tiles.size() < 99) return;

    int count = 0;
    int w = width();
    int h = height();
    int viewleft = (_mapx-w/2)/256;
    int viewright = (_mapx+w/2)/256;
    int viewtop = (_mapy-w/2)/256;
    int viewbottom = (_mapy+h/2)/256;
    QRect r(viewleft,viewtop,viewright-viewleft+1,viewbottom-viewtop+1);
    QHash<QPoint,QImage>::iterator i;
    for (i=tiles.begin(); i!=tiles.end(); ++i)
    {
        if (!r.contains(i.key()))
        {
            tiles.remove(i.key());
            count++;
        }
        if (count>10) break;
    }
}

void MapView::discardTile(QPoint& p)
{
    tiles.remove(p);
}

void MapView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int w = width();
    int h = height();

    int viewleft = (_mapx-w/2)/256;
    int viewright = (_mapx+w/2)/256;
    int viewtop = (_mapy-w/2)/256;
    int viewbottom = (_mapy+h/2)/256;
    for (int x = viewleft; x<=viewright; x++)
    {
        for (int y=viewtop; y<=viewbottom; y++)
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
{ _mapx = v; update(); }//     qDebug() << "mapx:     " << v; }

void MapView::setMapY(int v)
{ _mapy = v; update(); }//     qDebug() << "mapy:     " << v; }

void MapView::setZoom(int v)
{ _zoom = v; update(); }//     qDebug() << "zoom:     " << v; }

void MapView::onTileLoaded(const QPoint& p, QImage i)
{
    tiles[p]=i;
    update();
}

bool operator==(const Request& a, const Request& b)
{
    return ((a.point == b.point) && (a.url == b.url));
}

void TileLoader::onTileRequest(const QPoint& p, const QUrl& u)
{
    Request r(p,u);
    if (!requests.contains(r))
        requests.append(r);
}

void TileLoader::loadTile(const QPoint& p, const QUrl& u)
{
    QPoint pi(p.x()*256,p.y()*256);
    QSize  size(256,256);
    QRect  r(pi,size);
    QImage image;
    QImageReader reader;

    reader.setFileName(u.toLocalFile());
    reader.setClipRect(r);
    image = reader.read();

    emit tileLoaded(p,image);
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
        msleep(100);
    }
}

uint qHash(const QPoint& p)
{
    return p.x() * 17^p.y();
}

