#include "qmapstatusbar.h"
#include "ui.h"

QMapStatusBar::QMapStatusBar(QWidget *parent)
    : QToolButton(parent)
    , pen(Qt::black)
    , top("<no map>")
    , bottom("<0 0>")
{
	resize(250,40);
	QIcon icon;
        icon.addFile(QString(MAPRCDIR) + "statusbar.svg", QSize(), QIcon::Normal, QIcon::Off);
	setIcon(icon);
	setIconSize(QSize(250, 40));
}

QMapStatusBar::~QMapStatusBar()
{
}

void QMapStatusBar::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    QPainter painter(this);
    painter.setPen(pen);
    painter.setFont(QFont("Courier", 4 * height() / TEXTDIVIDER));
    painter.drawText(QRect(25, 4,200,15), Qt::AlignLeft, top);
    painter.drawText(QRect(25,20,200,15), Qt::AlignLeft, bottom);
}
