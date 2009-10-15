#ifndef QGAUGEWIDGET_H
#define QGAUGEWIDGET_H

#include <QWidget>
#include <QTimer>

class QGaugeWidget : public QWidget
{
    Q_OBJECT
public:
    QGaugeWidget(QWidget *parent = 0);
    ~QGaugeWidget();

signals:
    //void zoom();
    //void options();
    
    void singleTap();
    void doubleTap();
    void longTap();
    void drag(QPoint delta);

protected slots:
    //void timerExpired();
    void longTapTimeout();
    void singleTapTimeout();

protected:
    //void cancelTimer();
    //void startTimer();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    void QGaugeWidget::startLongTapTimer();
    void QGaugeWidget::cancelLongTapTimer();
    void QGaugeWidget::startSingleTapTimer();
    void QGaugeWidget::cancelSingleTapTimer();

private:
	
	static const int StIdle = 0;
	static const int StInitialTouch = 1;
	static const int StSingleTapPending = 2;
	static const int StLongTouch = 3;
	static const int StSecondTouch = 4;
	static const int StMoving = 5;
	
    QTimer longtaptimer;
    QTimer singletaptimer;
    QPoint previous;
    int state;
    //QTimer *timer;
    //bool longpress;
};

#endif // QGAUGEWIDGET_H
