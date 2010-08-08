#ifndef QCLOCKWIDGET_H
#define QCLOCKWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include "qgaugewidget.h"

class QClockWidget : public QGaugeWidget
{
    Q_OBJECT
    
public:
    QClockWidget(QWidget *parent = 0);

public slots:
    void TimeChanged();
    void Reset();
    void SetTime(const QTime& time);
    
protected:
    virtual void paintPlate(QPainter& painter);
    virtual void paintAnalog(QPainter& painter);
    virtual void paintTop(QPainter& painter);
    virtual void paintBottom(QPainter& painter);
    virtual void paintEvent(QPaintEvent *event);

private:
    QTime starttime;
    QTime analog;
    QTime top;
    QTime bottom;
    QTimer timer;
};

#endif // QCLOCKWIDGET_H
