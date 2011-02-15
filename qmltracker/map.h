#ifndef MAP_H
#define MAP_H

#include <QDeclarativeItem>
#include <QImage>

class Map : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(int mapx   READ mapX)
    Q_PROPERTY(int mapy   READ mapY)
    Q_PROPERTY(int mapw   READ mapW)
    Q_PROPERTY(int maph   READ mapH)
    Q_PROPERTY(int factor READ zoomFactor)
    Q_PROPERTY(int level  READ zoomLevel)

private:
    QImage *mapimage;
    int _x, _y, _z;

public:
    int    mapX()       const    { return _x; }
    int    mapY()       const    { return _y; }
    int    mapW()       const    { if (mapimage) return mapimage->size().width();  else return 0; }
    int    mapH()       const    { if (mapimage) return mapimage->size().height(); else return 0; }
    int    zoomLevel()  const    { return _z; }
    double zoomFactor() const;

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    explicit Map(QDeclarativeItem *parent = 0);

protected:
    void setMapX(int v);
    void setMapY(int v);
    void setZoom(int v);

signals:
    void mapLoaded(QString);

public slots:
    void zoomIn()              { setZoom(zoomLevel()+1); }
    void zoomOut()             { setZoom(zoomLevel()-1); }
    void moveTo(int x, int y);
    void pan(int dx, int dy);
};

QML_DECLARE_TYPE(Map)

#endif // MAP_H
