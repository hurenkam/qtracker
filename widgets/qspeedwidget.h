#ifndef QSPEEDWIDGET_H
#define QSPEEDWIDGET_H

#include <QWidget>
#include "qgaugewidget.h"

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

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QTimer *timer;
    int scale;
    int steps;
    double speed;
    double delta;
    double curspeed;
    double setspeed;
    double distance;
};

#endif // QSPEEDWIDGET_H
