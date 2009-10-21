#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "qgaugewidget.h"
#include "qmapmetadata.h"

class QMapWidget : public QGaugeWidget
{
    Q_OBJECT

public:
    QMapWidget(QWidget *parent = 0);
    ~QMapWidget();
    bool IsPositionOnMap() { return (meta && meta->IsPositionOnMap(latitude,longitude)); }

public slots:
    void updatePosition(double lat, double lon);
    void moveMap(int x, int y);
    void SelectMap();
    void FindMapsForCurrentPosition(QStringList& found);
    void MapSelected(QString map);
    void FollowGPS();
    
private slots:
    void zoomRepeat();

protected:
    bool LoadMap(QString filename);
    bool SelectBestMapForCurrentPosition();
    bool SetCursorToCurrentPosition()
    {
        if (meta)
            meta->Wgs2XY(latitude,longitude,x,y);
    }
    virtual void paintEvent(QPaintEvent *event);
    void CreateMapList();

private:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    
    static const int StNoMap = 0;
    static const int StScrolling = 1;
    static const int StFollowGPS = 2;
    int state;
    int zoom;

    double x, y;
    double latitude;
    double longitude;
    QImage* mapimage;
    QImage* bgimage;
    QImage* svgZoomIn;
    QImage* svgZoomOut;
    //QPushButton *btnZoomIn;
    //QPushButton *btnZoomOut;
    QMapMetaData *meta;
    QMapList maplist;
    int zooming;
    QTimer zoomtimer;
};

#endif // QMAPWIDGET_H
