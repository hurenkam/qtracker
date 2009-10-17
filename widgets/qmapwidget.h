#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QtXml>
#include <QTableWidget>
#include <QListWidget>
#include <QDialog>
#include "qgaugewidget.h"

class RefPoint
{
public:
    RefPoint() {}
    RefPoint(int ax,int ay, double alat, double alon)
            : x(ax), y(ay), lat(alat), lon(alon) {}
    int X() { return x; }
    int Y() { return y; }
    double Latitude() { return lat; }
    double Longitude() { return lon; }
    void setX(int ax) { x = ax; }
    void setY(int ay) { y = ay; }
    void setLatitude(double alat) { lat = alat; }
    void setLongitude(double alon) { lon = alon; }
private:
    int x;
    int y;
    double lat;
    double lon;
};

class QMapMetaData: public QObject
{
    Q_OBJECT
public:
    QMapMetaData(QString filename);
    ~QMapMetaData();
    void Calibrate();
    bool WgsArea(double& lat1, double& lon1, double& lat2, double& lon2);
    bool XY2Wgs(double ax, double ay, double& alat, double& alon);
    bool Wgs2XY(double alat, double alon, double& ax, double& ay);
    bool IsPositionOnMap(double alat, double alon);
    bool IsCalibrated() { return iscalibrated; }
    void SetSize(int w, int h) { width = w; height = h; }

private:
    void ReadMapElement(QXmlStreamReader& xml);
    void ReadResolutionElement(QXmlStreamReader& xml);
    void ReadRefpointElement(QXmlStreamReader& xml);

    void CalculateIndexesFromRefpoints(int i, int j);
    bool IsValidRefpointPair(int i, int j);

    RefPoint refpoints[10];
    int width;
    int height;
    int count;
    QString filename;

    int x;
    int y;
    double lat;
    double lon;
    double x2lon;
    double y2lat;
    double lon2x;
    double lat2y;
    bool iscalibrated;
};

typedef QMap<QString, QMapMetaData*> QMapList;

class QMapSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    QMapSelectionDialog(QMapList& maps, QWidget *parent = 0);
    ~QMapSelectionDialog();
};

class QMapWidget : public QGaugeWidget
{
    Q_OBJECT

public:
    QMapWidget(QWidget *parent = 0);
    ~QMapWidget();

public slots:
    void updatePosition(double lat, double lon);
    void followGPSPosition();
    void moveMap(int x, int y);
    void SelectMap();
    void SelectBestMapForCurrentPosition();
    void FindMapsForCurrentPosition(QStringList& found);
    void LoadMap(QString filename);
    void MapSelected(QListWidgetItem *item);

protected:
    virtual void paintEvent(QPaintEvent *event);
    void CreateMapList();

private:
    double latitude;
    double longitude;
    QPoint cursor;
    QImage *mapimage;
    QImage *bgimage;
    bool scrolling;
    QMapMetaData *meta;
    //bool onmap;
    QMapList maplist;
};

#endif // QMAPWIDGET_H
