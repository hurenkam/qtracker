#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QSystemDeviceInfo>
#include <string>
#include "qgaugewidget.h"

namespace QtMobility
{
    class QGeoCoordinate;
    class QGeoPositionInfo;
}
using namespace QtMobility;

class QWidget;
class QSettings;
class WayPoint;
class MapMetaData;
class Track;
class Route;

class ScreenPos
{
public:
	int x;
	int y;
};

class QMapWidget : public QGaugeWidget
{
    Q_OBJECT

public:
    QMapWidget(QSettings& s, QWidget *parent = 0);
    ~QMapWidget();
    bool IsPositionOnMap();
    bool IsPositionOnScreen(const WayPoint& wpt);
    ScreenPos PositionOnScreen(const WayPoint& wpt);

signals:
    void zoomin();
    void zoomout();
    void waypoint();
    void track();
    void options();
    void datum();

public slots:
    void UpdatePosition(const QGeoPositionInfo& info);
    void updatePosition(const QGeoCoordinate& pos);
    void moveMap(int x, int y);
    void MapSelected(QString map);
    void RefpointSelected(QString name, double lat, double lon);
    void FollowGPS();

    void ShowMapDialog();
    void ShowWaypointDialog();
    
    void ShowTrackDialog();
    void ShowTrack(const QString& name);
    void HideTrack(const QString& name);
    void ShowTrackPoint(const WayPoint& w);
    
    void ShowRouteDialog();
    void ShowRoute(const QString& name);
    void HideRoute(const QString& name);
    void ShowRouteSegment(const WayPoint& from, const WayPoint& to);

private slots:
    void zoomRepeat();
    void zoomIn();
    void zoomOut();
    void batteryStatusChanged(QSystemDeviceInfo::BatteryStatus status);
    
protected:
    void FindMapsForCurrentPosition(QStringList& found);
    bool LoadMap(QString filename);
    bool SelectBestMapForCurrentPosition();
    bool SetCursorToCurrentPosition();
    std::string getRepresentation(double lat, double lon);
    virtual void paintTrack(Track* t);
    virtual void paintRoute(Route* r);
    virtual void paintBackground(QPainter& painter);
    virtual void paintMap(QPainter& painter);
    virtual void paintWaypoints(QPainter& painter);
    virtual void paintWidgets(QPainter& painter);
    virtual void paintDot(QPainter& painter,int x,int y,QColor c);
    virtual void paintBar(QPainter& painter);
    virtual void paintEvent(QPaintEvent *event);
    void CreateMapList();

private:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    QSystemDeviceInfo devinfo;
    static const int StNoMap = 0;
    static const int StScrolling = 1;
    static const int StFollowGPS = 2;
    int state;
    int zoom;

    double x, y;
    double latitude;
    double longitude;
    double altitude;
    QImage* mapimage;
    QImage* bgimage;
    QImage* svgZoomIn;
    QImage* svgZoomOut;
    QImage* svgOptions;
    QImage* svgFlag;
    QImage* svgHiker;
    QImage* svgBar;
    QImage* svgLocator;
    QImage* svgWptGreen;
    bool onmap;
    MapMetaData *meta;
    int zooming;
    QTimer zoomtimer;
    QString mapname;
    bool ismapdirty;
    QSettings& settings;
};

#endif // QMAPWIDGET_H
