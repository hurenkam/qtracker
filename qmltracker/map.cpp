#include "map.h"
#include <QRectF>
#include <QPainter>
#include <QDebug>

#ifdef WIN32
    #define MAPDIR "c:\\"
    #define MAPFILE "map.jpg"
#else
    #define MAPDIR "e:\\data\\qmltracker\\"
    #define MAPFILE "map.jpg"
#endif

static double zoomlevels[] = { 3, 2, 1, 0.5, 0.3 };

Map::Map(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), _x(0), _y(0), _z(2)
{
    qDebug() << "creating map";

    mapimage = new QImage();
    bool result = mapimage->load(MAPDIR MAPFILE);
    if (!result)
    {
        qDebug() << "unable to load mapimage";
        delete mapimage;
        mapimage = 0;
        return;
    }
    setFlag(QGraphicsItem::ItemHasNoContents,false);
    setFlag(QGraphicsItem::ItemHasNoContents,false);
    _x = mapimage->size().width()/2;
    _y = mapimage->size().height()/2;
}

double
Map::zoomFactor() const
{
    return zoomlevels[_z];
}

void
Map::setZoom(int v)
{
    if ((v >= 0) && (v<=4))
        _z = v;
    update();
    //qDebug() << "New zoomlevel: " << _z;
}

void
Map::setMapX(int v)
{
    if (!mapimage) return;
    if ((v<0)||(v>=mapimage->size().width())) return;

    _x = v;
}

void
Map::setMapY(int v)
{
    if (!mapimage) return;
    if ((v<0)||(v>=mapimage->size().height())) return;

    _y = v;
}

void
Map::moveTo(int x, int y)
{
    setMapX(x);
    setMapY(y);
    update();
    emit positionChanged();
    //qDebug() << "New map position: " << _x << "," << _y;
}

void
Map::pan(int dx, int dy)
{
    double z = zoomlevels[_z];
    moveTo(mapX()+dx*z,mapY()+dy*z);
}

Q_INVOKABLE QString
Map::position()
{
    QString s = QString::number(_x,'g',8) + "  " + QString::number(_y,'g',8);
    return s;
}

void
Map::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    double z = zoomFactor();

    if (!mapimage) { qDebug() << "no mapimage"; return; }
    if (!painter)  { qDebug() << "no painter";  return; }

    double w = width();
    double h = height();
    double x = (double) _x - w*z/2;
    double y = (double) _y - w*z/2;
    QRect src((int) x, (int) y, (int) ((double)w * z),(int) ((double)h * z));
    QRect tgt(0, 0, w, h);
    //qDebug() << "painting map area; width: " << w << " height: " << h;
    //if (smooth())
    //    painter->setRenderHint(QPainter::Antialiasing,true);
    //painter->setWindow(0,0,w,h);
    painter->drawImage(tgt, *mapimage, src);
}
