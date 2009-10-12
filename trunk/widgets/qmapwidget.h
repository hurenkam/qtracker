#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QWidget>

class QMapWidget : public QWidget {
    Q_OBJECT
public:
    QMapWidget(QWidget *parent = 0);
    ~QMapWidget();
protected:
    virtual void paintEvent(QPaintEvent *event);
};

#endif // QMAPWIDGET_H
