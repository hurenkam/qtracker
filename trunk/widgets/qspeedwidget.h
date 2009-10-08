#ifndef QSPEEDWIDGET_H
#define QSPEEDWIDGET_H

#include <QWidget>

class QSpeedWidget : public QWidget
{
    Q_OBJECT
public:
    QSpeedWidget(QWidget *parent);
    ~QSpeedWidget();
    void SetSpeed(double s);
    void SetDistance(double d);
    void SetScale(int s);
    
public slots:
    void timerStep();

protected:
    void changeEvent(QEvent *e);
    virtual void paintEvent(QPaintEvent *event);

private:
    double speed;
    int scale;
    int steps;
    double setspeed;
    double curspeed;
    double delta;
    double distance;
};

#endif // QSPEEDWIDGET_H
