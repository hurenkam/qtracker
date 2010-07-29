#ifndef QHEADINGWIDGET_H
#define QHEADINGWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include "qgaugewidget.h"
#include "waypointlist.h"

using namespace QtMobility;

class QHeadingWidget : public QGaugeWidget
{
    Q_OBJECT
public:
    QHeadingWidget(QWidget *parent = 0);

    void SetDial(double v);
    void SetNeedle(double v);
    void SetRing(double v);

public slots:
    void timerStep();
    void SelectOptions();
	void UpdatePosition(const QGeoPositionInfo& info);
	void ReadSettings();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QSettings settings;
	WayPoint monitor;
	QGeoPositionInfoSource* possource;
    QTimer *timer;
    double setdial;
    double curdial;
    double deltadial;
    int    stepsdial;
    double setneedle;
    double curneedle;
    double deltaneedle;
    int    stepsneedle;
    double setring;
    double curring;
    double deltaring;
    int    stepsring;
    QString wptname;
    QString trkname;
    QString rtename;
    int montype;
    int source;
    int view;
};

#endif // QHEADINGWIDGET_H
