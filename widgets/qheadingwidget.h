#ifndef QHEADINGWIDGET_H
#define QHEADINGWIDGET_H

#include <QWidget>
#include <QSvgRenderer>

class QHeadingWidget : public QWidget {
    Q_OBJECT
public:
    QHeadingWidget(QWidget *parent = 0);
    ~QHeadingWidget();

    void SetDial(double v);
    void SetNeedle(double v);

public slots:
        void timerStep();

protected:
    virtual void paintEvent(QPaintEvent *event);
    void changeEvent(QEvent *e);

private:
    QTimer *timer;
    QSvgRenderer *svgdial;
    QSvgRenderer *svgneedle;
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
