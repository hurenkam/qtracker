#ifndef QHEADINGWIDGET_H
#define QHEADINGWIDGET_H

#include <QWidget>
#include "qgaugewidget.h"

class QHeadingWidget : public QGaugeWidget
{
    Q_OBJECT
public:
    QHeadingWidget(QWidget *parent = 0);

    void SetDial(double v);
    void SetNeedle(double v);

public slots:
        void timerStep();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QTimer *timer;
    double setdial;
    double curdial;
    double deltadial;
    int    stepsdial;
    double setneedle;
    double curneedle;
    double deltaneedle;
    int    stepsneedle;
};

#endif // QHEADINGWIDGET_H
