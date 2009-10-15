#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QtXml>

class RefPoint
{
public:
    RefPoint() {}
    RefPoint(int ax,int ay, double alat, double alon)
            : x(ax), y(ay), lat(alat), lon(alon) {}
    double X() { return x; }
    double Y() { return y; }
    double Latitude() { return lat; }
    double Longitude() { return lon; }
    void setX(double ax) { x = ax; }
    void setY(double ay) { y = ay; }
    void setLatitude(double alat) { lat = alat; }
    void setLongitude(double alon) { lon = alon; }
private:
    double x;
    double y;
    double lat;
    double lon;
};

class QMapMetaData : public QObject
{
    Q_OBJECT
public:
    QMapMetaData(QString filename);
    ~QMapMetaData();

private:
    void ReadMapElement(QXmlStreamReader& xml);
    void ReadResolutionElement(QXmlStreamReader& xml);
    void ReadRefpointElement(QXmlStreamReader& xml);

    RefPoint refpoints[10];
    int width;
    int height;
    int count;
    QString filename;
};

class QMapWidget : public QWidget {
    Q_OBJECT
public:
    QMapWidget(QWidget *parent = 0);
    ~QMapWidget();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
private:
    void moveMap(int x, int y);
    QPoint start;
    QPoint stop;
    QPoint current;
    QImage *mapimage;
    QImage *bgimage;
};

#endif // QMAPWIDGET_H
