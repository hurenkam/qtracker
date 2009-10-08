#ifndef QALTITUDEWIDGET_H
#define QALTITUDEWIDGET_H

#include <QWidget>

class QAltitudeWidget : public QWidget
{
    Q_OBJECT
public:
    QAltitudeWidget(QWidget *parent);
    ~QAltitudeWidget();
    void SetAltitude(double a);
    //void SetMin(double m);
    //void SetMax(double m);

public slots:
	void timerStep();

protected:
    void changeEvent(QEvent *e);
    virtual void paintEvent(QPaintEvent *event);

private:
    double set;
    double current;
    double delta;
    int    steps;
    
    double min;
    double max;
    bool   valid;
};

#endif // QALTITUDEWIDGET_H
