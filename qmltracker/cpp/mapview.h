#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QDeclarativeItem>
#include <QImageReader>
#include <QThread>

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
    void tileLoaded(const QPoint& p,QImage i);

public slots:
    void onTileRequest(const QPoint& p, const QUrl& u);

private:
    QList<Request> requests;

public:
    void loadTile(const QPoint& p, const QUrl& u);
    TileLoader(QObject *parent=0) : QThread(parent) {}
    void run();
};

class MapView : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl    filename   READ filename    WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(QSize   filesize   READ filesize                      NOTIFY filesizeChanged)
    Q_PROPERTY(int     mapx       READ mapx        WRITE setMapX     NOTIFY mapxChanged)
    Q_PROPERTY(int     mapy       READ mapy        WRITE setMapY     NOTIFY mapyChanged)
    Q_PROPERTY(int     zoom       READ zoom        WRITE setZoom     NOTIFY zoomChanged)

signals:
    void filenameChanged();
    void mapxChanged();
    void mapyChanged();
    void zoomChanged();
    void filesizeChanged();
    void loadTile(const QPoint&, const QUrl&);

public:
    Q_INVOKABLE void invalidate();
    explicit MapView(QDeclarativeItem *parent = 0);

    QUrl    filename()          { return _filename;  }
    QSize   filesize()          { return _filesize;  }
    int     mapx()              { return _mapx;      }
    int     mapy()              { return _mapy;      }
    int     zoom()              { return _zoom;      }

    void setFilename(QUrl u);
    void setMapX(int v);
    void setMapY(int v);
    void setZoom(int v);

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void renderEmpty(QPainter *painter, QPoint& p);
    void renderTile(QPainter *painter, QPoint& p);
    //bool loadTile(QPoint& p);
    void discardTile(QPoint& p);

private:
    QUrl    _filename;
    int     _mapx;
    int     _mapy;
    int     _zoom;
    QSize                  _filesize;
    QSize                  _tilesize;
    QImageReader           reader;
    QImage                 empty;
    QHash<QPoint, QImage>  tiles;
    TileLoader* mythread;

signals:

public slots:
    void onTileLoaded(const QPoint& p, QImage i);
};

#endif // MAPVIEW_H
