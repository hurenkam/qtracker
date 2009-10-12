#ifndef QSATVIEWWIDGET_H
#define QSATVIEWWIDGET_H

#include <QWidget>
#include "qgaugewidget.h"
//#include "xqlocation.h"

class QSatViewWidget : public QGaugeWidget
{
    Q_OBJECT
public:
    QSatViewWidget(QWidget *parent);

public:
        //void SetSatInfo(XQSatInfo &satinfo);
        void SetSatInfo(int id, int strength, double azimuth, double elevation, bool inuse);
        void ClearSatInfo(int id);

public slots:
    void timerExpired();

protected:
    void paintSatInfo(QPainter &painter, int id);
    virtual void paintEvent(QPaintEvent *event);

private:
    bool   inuse[32];
    int    strength[32];
    double azimuth[32];
    double elevation[32];
};

#endif // QSATVIEWWIDGET_H
