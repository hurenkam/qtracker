#ifndef QSPEEDWIDGET_H
#define QSPEEDWIDGET_H

#include <QWidget>
#include "qgaugewidget.h"

namespace QtMobility
{
    class QGeoPositionInfo;
    class QGeoCoordinate;
}
using namespace QtMobility;

class QSpeedWidget : public QGaugeWidget
{
    Q_OBJECT
public:
    QSpeedWidget(QWidget *parent);
    void SetSpeed(double s);
    void SetDistance(double d);
    void SetScale(int s);

public slots:
    void timerStep();
    void Reset();
	void UpdatePosition(const QGeoPositionInfo& info);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QGeoCoordinate* prevpos;
    QTimer *timer;
    int scale;
    int steps;
    double speed;
    double delta;
    double curspeed;
    double setspeed;
    double distance;
    double mindist;
};

#endif // QSPEEDWIDGET_H
