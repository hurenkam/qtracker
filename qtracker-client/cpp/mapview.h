#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QDeclarativeItem>
#include <QImageReader>
#include <QThread>
#include "database.h"

uint qHash(const QPoint& p);

class Request
{
public:
    Request(const QPoint& p, const QUrl& u)
    {
        point = p;
        url = u;
    }
    Request() {}

    QPoint point;
    QUrl url;
};

class TileLoader: public QThread
{
    Q_OBJECT
signals:
    void invalidate();
    void reload(const QPoint&, const QUrl&);
    void tileLoaded(const QPoint& p,QImage i);

public slots:
    void onTileRequest(const QPoint& p, const QUrl& u);

private:
    QList<Request> requests;

public:
    void loadTile(const QPoint& p, const QUrl& u);
    TileLoader(QObject *parent=0);
    void run();
};

class MapView : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QString name       READ name                          NOTIFY nameChanged)
    Q_PROPERTY(QUrl    filename   READ filename                      NOTIFY filenameChanged)
    Q_PROPERTY(QSize   filesize   READ filesize                      NOTIFY filesizeChanged)
    Q_PROPERTY(int     mapx       READ mapx        WRITE setMapX     NOTIFY mapxChanged)
    Q_PROPERTY(int     mapy       READ mapy        WRITE setMapY     NOTIFY mapyChanged)
    Q_PROPERTY(double  scale      READ scale       WRITE setScale    NOTIFY scaleChanged)
    Q_PROPERTY(int     trackid    READ trackid     WRITE setTrackid  NOTIFY trackidChanged)
    Q_PROPERTY(int     mapid      READ mapid       WRITE setMapid    NOTIFY mapidChanged)

signals:
    void nameChanged();
    void filenameChanged();
    void mapxChanged();
    void mapyChanged();
    void scaleChanged();
    void trackidChanged();
    void mapidChanged();
    void filesizeChanged();
    void loadTile(const QPoint&, const QUrl&);

public:
    Q_INVOKABLE void clearcache();
    Q_INVOKABLE void invalidate();
    Q_INVOKABLE void setArea(QVariantMap a);
    Q_INVOKABLE QVariant x2lon(QVariant x)   { return QVariant( _map? _map->mapx2lon(x.toInt())   : 0.0 ); }
    Q_INVOKABLE QVariant y2lat(QVariant y)   { return QVariant( _map? _map->mapy2lat(y.toInt())   : 0.0 ); }
    Q_INVOKABLE QVariant lon2x(QVariant lon) { return QVariant( _map? _map->lon2mapx(lon.toInt()) : 0   ); }
    Q_INVOKABLE QVariant lat2y(QVariant lat) { return QVariant( _map? _map->lat2mapy(lat.toInt()) : 0   ); }

    explicit MapView(QDeclarativeItem *parent = 0);

    QString name()              { return _map? _map->name(): "<unknown>"; }
    QUrl    filename()          { return _filename;  }
    QSize   filesize()          { return _filesize;  }
    int     mapx()              { return _mapx;      }
    int     mapy()              { return _mapy;      }
    int     scale()             { return _scale;     }
    int     trackid()           { return _trackid;   }
    int     mapid()             { return _mapid;     }

    void setMapX(int v);
    void setMapY(int v);
    void setScale(double v);
    void setTrackid(int id);
    void setMapid(int id);

protected:
    void setFilename(QUrl u);

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void renderEmpty(QPainter *painter, QPoint& p);
    void renderTile(QPainter *painter, QPoint& p);
    void renderWaypoints(QPainter *painter);
    void renderWaypoints(QPainter *painter, QPoint& p);
    void discardTile(const QPoint& p);
    Waypoint MapXY2Waypoint(const QPoint& p);
    QPoint Waypoint2MapXY(const Waypoint& w);
    QPoint MapXY2ViewXY(const QPoint& p);
    QRect viewTiles();
    QRect mapTiles();
    QRect sourceArea(const QPoint& p);
    QRect targetArea(const QPoint& p);
    Area geoArea(const QPoint& p);

private:
    QUrl                   _filename;
    int                    _mapx;
    int                    _mapy;
    int                    _trackid;
    int                    _mapid;
    double                 _scale;
    QSize                  _filesize;
    Map*                   _map;
    QImageReader           reader;
    QImage                 empty;
    QHash<QPoint, QImage>  tiles;
    TileLoader*            mythread;
    Area*                  area;
    WaypointList           waypoints;

public slots:
    void onInvalidate() { invalidate(); }
    void onTileLoaded(const QPoint& p, QImage i);
};

#endif // MAPVIEW_H
