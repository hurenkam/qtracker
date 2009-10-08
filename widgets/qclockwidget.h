#ifndef QCLOCKWIDGET_H
#define QCLOCKWIDGET_H

#include <QtGui/QWidget>

class QClockWidget : public QWidget {
    Q_OBJECT
public:
    QClockWidget(QWidget *parent = 0);
    ~QClockWidget();

    void SetTime(int h,int m,int s);

protected:
    void changeEvent(QEvent *e);
    virtual void paintEvent(QPaintEvent *event);

private:
    int hours;
    int minutes;
    int seconds;
};

#endif // QCLOCKWIDGET_H
