#ifndef QGAUGEWIDGET_H
#define QGAUGEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QSettings>

class QGaugeWidget : public QWidget
{
    Q_OBJECT
public:
    QGaugeWidget(QWidget *parent = 0);
    ~QGaugeWidget();
    void SetDragMin(int aminx, int aminy) { minx=aminx; miny=aminy; }
    virtual void SaveSettings();

signals:
    void singleTap();
    void doubleTap();
    void longTap();
    void drag(int x, int y);

protected slots:
    void longTapTimeout();
    void singleTapTimeout();
    //void longMoveTimeout();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    void startSingleTapTimer();
    void cancelSingleTapTimer();
    void startLongTapTimer();
    void cancelLongTapTimer();
    //void QGaugeWidget::startLongMoveTimer();
    //void QGaugeWidget::cancelLongMoveTimer();

protected:
    QSettings settings;
    
private:
    static const int StIdle = 0;
    static const int StInitialTouch = 1;
    static const int StSingleTapPending = 2;
    static const int StLongTouch = 3;
    static const int StSecondTouch = 4;
    static const int StMoving = 5;
    //static const int StLongMove = 6;

    QTimer longtaptimer;
    //QTimer longmovetimer;
    QTimer singletaptimer;
    QPoint previous;
    int state;
    int minx;
    int miny;
};

#endif // QGAUGEWIDGET_H
