#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QDeclarativeItem>
#include <QImageReader>
#include <QThread>

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
    Q_PROPERTY(QUrl    filename   READ filename    WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(QSize   filesize   READ filesize                      NOTIFY filesizeChanged)
    Q_PROPERTY(int     mapx       READ mapx        WRITE setMapX     NOTIFY mapxChanged)
    Q_PROPERTY(int     mapy       READ mapy        WRITE setMapY     NOTIFY mapyChanged)
    Q_PROPERTY(double  scale      READ scale       WRITE setScale    NOTIFY scaleChanged)

signals:
    void filenameChanged();
    void mapxChanged();
    void mapyChanged();
    void scaleChanged();
    void filesizeChanged();
    void loadTile(const QPoint&, const QUrl&);

public:
    Q_INVOKABLE void invalidate();
    explicit MapView(QDeclarativeItem *parent = 0);

    QUrl    filename()          { return _filename;  }
    QSize   filesize()          { return _filesize;  }
    int     mapx()              { return _mapx;      }
    int     mapy()              { return _mapy;      }
    int     scale()             { return _scale;     }

    void setFilename(QUrl u);
    void setMapX(int v);
    void setMapY(int v);
    void setScale(double v);

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void renderEmpty(QPainter *painter, QPoint& p);
    void renderTile(QPainter *painter, QPoint& p);
    void discardTile(const QPoint& p);
    QRect viewTiles();
    QRect mapTiles();
    QRect sourceArea(const QPoint& p);
    QRect targetArea(const QPoint& p);

private:
    QUrl                   _filename;
    int                    _mapx;
    int                    _mapy;
    double                 _scale;
    QSize                  _filesize;
    QImageReader           reader;
    QImage                 empty;
    QHash<QPoint, QImage>  tiles;
    TileLoader* mythread;

public slots:
    void onInvalidate() { invalidate(); }
    void onTileLoaded(const QPoint& p, QImage i);
};

#endif // MAPVIEW_H
