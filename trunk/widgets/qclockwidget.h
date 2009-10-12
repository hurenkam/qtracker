#ifndef QCLOCKWIDGET_H
#define QCLOCKWIDGET_H

#include "qgaugewidget.h"
#include <QtGui/QWidget>

class QClockWidget : public QGaugeWidget
{
    Q_OBJECT
public:
    QClockWidget(QWidget *parent = 0);
    void SetTime(int h,int m,int s);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    int hours;
    int minutes;
    int seconds;
};

#endif // QCLOCKWIDGET_H
