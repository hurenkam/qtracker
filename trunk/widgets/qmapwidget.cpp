#include <QPainter>
#include "ui.h"
#include "qmapwidget.h"

QMapWidget::QMapWidget(QWidget *parent)
    : QWidget(parent)
{
}

QMapWidget::~QMapWidget()
{
}

void QMapWidget::paintEvent(QPaintEvent *)
{
    double w = width();
    double x = 0;
    double h = height();
    double y = 0;

    QPainter painter(this);
    QRectF source(0, 0, 400, 360);
    QRectF target(0, 0, w, h);

    painter.drawImage(target, svgMap, source);
}

