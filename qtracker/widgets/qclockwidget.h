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

    enum Type
	{
        CurrentTime = 0,
        TripTime,
        ArrivalTime,
        RemainingTime
	};
    
public slots:
    void Reset();
    void SetStartTime     (const QDateTime& time) { starttime = time; } 
    void SetRemainingTime (long seconds)          { remainingtime = seconds; } 
    void SelectOptions();
    void ReadSettings();
    
protected:
    virtual void paintPlate(QPainter& painter);
    virtual void paintAnalog(QPainter& painter);
    virtual void paintTop(QPainter& painter);
    virtual void paintBottom(QPainter& painter);
    virtual void paintEvent(QPaintEvent *event);
    
    QTime GetTime(Type t);

private:
    QDateTime starttime;
    long   remainingtime;
    Type   analog;
    Type   top;
    Type   bottom;
    QTimer timer;
};

#endif // QCLOCKWIDGET_H
