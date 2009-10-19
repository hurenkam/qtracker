#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QWidget>
#include "qgaugewidget.h"
#include "qmapmetadata.h"

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

protected:
    virtual void paintEvent(QPaintEvent *event);
    void CreateMapList();

private:
    double latitude;
    double longitude;
    QPoint cursor;
    QImage* mapimage;
    QImage* bgimage;
    bool scrolling;
    QMapMetaData *meta;
    //bool onmap;
    QMapList maplist;
};

#endif // QMAPWIDGET_H
