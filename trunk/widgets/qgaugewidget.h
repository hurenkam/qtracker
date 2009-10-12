#ifndef QGAUGEWIDGET_H
#define QGAUGEWIDGET_H

#include <QWidget>

class QGaugeWidget : public QWidget
{
    Q_OBJECT
public:
    QGaugeWidget(QWidget *parent = 0);
    ~QGaugeWidget();

signals:
    void zoom();
    void options();

protected slots:
    void timerExpired();

protected:
    void cancelTimer();
    void startTimer();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QTimer *timer;
    bool longpress;
};

#endif // QGAUGEWIDGET_H
