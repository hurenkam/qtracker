#ifndef QALTITUDEWIDGET_H
#define QALTITUDEWIDGET_H

#include <QWidget>
#include "qgaugewidget.h"

class QAltitudeWidget : public QGaugeWidget
{
    Q_OBJECT
public:
    QAltitudeWidget(QWidget *parent);
    void SetAltitude(double a);

public slots:
        void timerStep();

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
