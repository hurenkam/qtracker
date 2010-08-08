#ifndef QLEVELWIDGET_H
#define QLEVELWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include "qgaugewidget.h"

class QLevelWidget : public QGaugeWidget
{
    Q_OBJECT
    
public:
    QLevelWidget(QWidget *parent = 0);
    void SetRange(int id, double minlevel, double maxlevel);

public slots:
    void Reset();
    void UpdateLeft   (double value) { UpdateLevel(0,value); }
    void UpdateRight  (double value) { UpdateLevel(1,value); }
    void UpdateTop    (double value) { UpdateLevel(2,value); }
    void UpdateBottom (double value) { UpdateLevel(3,value); }
    
protected:
    virtual void UpdateLevel(int id, double value);
    virtual void paintLevel(int id, QPainter& p);
    virtual void paintPlate(QPainter& p);
    virtual void paintTop(QPainter& p);
    virtual void paintEvent(QPaintEvent *event);

private:
    QTimer timer;
    double min[4];
    double max[4];
    double current[4];
};

#endif // QCLOCKWIDGET_H
