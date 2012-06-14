#ifndef QSATVIEWWIDGET_H
#define QSATVIEWWIDGET_H

#include <QWidget>
#include <QGeoSatelliteInfo>
#include "qgaugewidget.h"
//#include "xqlocation.h"

using namespace QtMobility;

class QSatViewWidget : public QGaugeWidget
{
    Q_OBJECT
public:
    QSatViewWidget(QWidget *parent);

public slots:
    void timerExpired();

protected:
    void paintSatInfo(QPainter &painter, const QGeoSatelliteInfo& info, bool inuse);
    virtual void paintEvent(QPaintEvent *event);

};

#endif // QSATVIEWWIDGET_H
