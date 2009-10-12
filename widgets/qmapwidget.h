#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QWidget>
#include <QPoint>

class QMapWidget : public QWidget {
    Q_OBJECT
public:
    QMapWidget(QWidget *parent = 0);
    ~QMapWidget();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
private:
    void moveMap(int x, int y);
    QPoint start;
    QPoint stop;
    QPoint current;
    QWidget *mapimage;
};

#endif // QMAPWIDGET_H
