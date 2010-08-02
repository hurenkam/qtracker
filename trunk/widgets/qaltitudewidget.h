#ifndef QALTITUDEWIDGET_H
#define QALTITUDEWIDGET_H

#include <QWidget>
#include "qgaugewidget.h"

namespace QtMobility
{
	class QGeoPositionInfo;
}
using namespace QtMobility;

class QAltitudeWidget : public QGaugeWidget
{
    Q_OBJECT
public:
    QAltitudeWidget(QWidget *parent);

public slots:
	void timerStep();
	void reset();
    void SetAltitude(double a);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QTimer *timer;
    double current;
    double set;
    double delta;
    int    steps;

    double min;
    double max;
    bool   valid;
};

#endif // QALTITUDEWIDGET_H
