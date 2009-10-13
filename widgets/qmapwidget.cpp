#include <QPainter>
#include "ui.h"
#include "qmapwidget.h"
#include <iostream.h>

QMapWidget::QMapWidget(QWidget *parent)
    : QWidget(parent)
    , current(QPoint(0,0))
{
    //mapimage = new QWidget(this);
    //mapimage->setObjectName(QString::fromUtf8("map"));
    //mapimage->setGeometry(QRect(5, 5, width()-5, height()-5));
	bgimage = new QImage(QString(UIDIR "map.svg"));
	mapimage = new QImage(QString(UIDIR "map.jpg"));
}

QMapWidget::~QMapWidget()
{
}

void QMapWidget::mousePressEvent(QMouseEvent *event)
{
    //cout << "MousePressEvent " << event->pos().x() << " " << event->pos().y() << "\n";
    start = event->pos();
}

void QMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    //cout << "MouseDragEvent " << event->pos().x() << " " << event->pos().y() << "\n";
    stop = event->pos();
    moveMap(stop.x() - start.x(), stop.y() - start.y());
    start = event->pos();
}

void QMapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //cout << "MouseReleaseEvent " << event->pos().x() << " " << event->pos().y() << "\n";
    stop = event->pos();
    moveMap(stop.x() - start.x(), stop.y() - start.y());
}

void QMapWidget::moveMap(int x, int y)
{
    current.setX(current.x()-x);
    current.setY(current.y()-y);
    update();
}

void QMapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    double w = width();
    double x = 0;
    double h = height();
    double y = 0;

    QPainter painter(this);
    QRectF source(0, 0, 400, 360);
    QRectF target(0, 0, w, h);

    painter.drawImage(target, *bgimage, source);
    source = QRectF(current.x(), current.y(), w-40, h-40);
    target = QRectF(20, 20, w-40, h-40);
    painter.drawImage(target, *mapimage, source);
}

