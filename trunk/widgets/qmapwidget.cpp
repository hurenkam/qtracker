#include <QPainter>
#include <QFlags>
#include <cmath>
#include "ui.h"
#include "qmapwidget.h"
#include "qmapselectiondialog.h"

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG( a )


#ifdef Q_OS_SYMBIAN
#define MAPDIR "c:/data/qtracker/maps/"
#else
#define MAPDIR "/Users/hurenkam/workspace/qtracker/maps/"
#endif

QMapWidget::QMapWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , latitude(0)
    , longitude(0)
    , cursor(QPoint(0,0))
    , scrolling(true)
    , meta(0)
    , mapimage(0)
    , bgimage(new QImage(QString(UIDIR "map.svg")))
{
    connect(this, SIGNAL(drag(int,int)), this, SLOT(moveMap(int,int)));
    connect(this, SIGNAL(singleTap()), this, SLOT(followGPSPosition()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(SelectMap()));

    CreateMapList();
}

QMapWidget::~QMapWidget()
{
}

void QMapWidget::CreateMapList()
{
    QDir directory = QDir(MAPDIR);

    QStringList files = directory.entryList(QStringList(QString("*.xml")),
                                 QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i)
    {
        LOG( "QMapWidget::CreateMapList() " << files[i].toStdString() << "\n"; )
        maplist[files[i]] = new QMapMetaData(QString(MAPDIR) + files[i]);
    }
}

void QMapWidget::SelectBestMapForCurrentPosition()
{
    LOG( "QMapWidget::SelectBestMapForCurrentPosition()\n"; )
    QStringList found;
    FindMapsForCurrentPosition(found);
    if (found.size() > 0)
    {
        LOG( "QMapWidget::SelectBestMapForCurrentPosition(): " << found[0].toStdString() << "\n"; )
        // for now select first entry
        LoadMap(found[0]);
    }
}

void QMapWidget::FindMapsForCurrentPosition(QStringList &found)
{
    LOG( "QMapWidget::FindMapsForCurrentPosition()\n"; )
    QStringList keys = maplist.keys();
    for (int i=0; i<keys.size(); ++i)
    {
        if (maplist[keys[i]]->IsPositionOnMap(latitude,longitude))
        {
            LOG( "QMapWidget::FindMapsForCurrentPosition(): " << keys[i].toStdString() << "\n"; )
            found.append(keys[i]);
        }
    }
}

void QMapWidget::LoadMap(QString filename)
{
    LOG( "QMapWidget::LoadMap(): " << filename.toStdString() << "\n"; )
    meta = maplist[filename];
    filename.remove(filename.size()-4,4);
    filename.append(".jpg");
    if (!mapimage)
        delete mapimage;
    mapimage = new QImage();
    bool result = mapimage->load(QString(MAPDIR) + filename);
    if (!result)
        {
                QMessageBox msg;
                msg.setText(QString("Unable to load map ") + filename);
                msg.setIcon(QMessageBox::Warning);
                msg.exec();
        }
    else
        {
                meta->SetSize(mapimage->width(),mapimage->height());
                meta->Calibrate();
        }
    update();
}

void QMapWidget::SelectMap()
{
    QMapSelectionDialog *dialog = new QMapSelectionDialog(maplist);
    connect(dialog,SIGNAL(selectmap(QString)),this,SLOT(LoadMap(QString)));
    dialog->setModal(true);
#ifdef Q_OS_SYMBIAN
    dialog->showFullScreen();
#else
    dialog->show();
#endif
}

void QMapWidget::updatePosition(double lat, double lon)
{
    //LOG( "QMapWidget::updatePosition()\n"; )
    latitude = lat;
    longitude = lon;
    if (scrolling) return;
    if (!meta) return;
    if (!meta->IsCalibrated()) return;
    if (!meta->IsPositionOnMap(lat,lon)) return;

    //LOG( "QMapWidget::updatePosition() OnMap!\n"; )
    double x,y;
    meta->Wgs2XY(lat,lon,x,y);
    cursor.setX(x);
    cursor.setY(y);
    update();
}

void QMapWidget::moveMap(int x, int y)
{
    scrolling = true;
    cursor.setX(cursor.x()-x);
    cursor.setY(cursor.y()-y);
    update();
}

void QMapWidget::followGPSPosition()
{
    LOG( "QMapWidget::followGPSPosition()\n"; )
    if (!meta || !meta->IsPositionOnMap(latitude,longitude))
        SelectBestMapForCurrentPosition();

    scrolling = false;
    updatePosition(latitude,longitude);
}

void QMapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    double w = width();
    double h = height();
    double s = h / 36;

    QPainter painter(this);
    QRectF source(0, 0, 400, 360);
    QRectF target(0, 0, w, h);

    painter.drawImage(target, *bgimage, source);
    source = QRectF(20+cursor.x()-w/2, 20+cursor.y()-h/2, w-40, h-40);
    target = QRectF(20, 20, w-40, h-40);
    if (mapimage)
        painter.drawImage(target, *mapimage, source);

    if (scrolling || !meta || !meta->IsPositionOnMap(latitude,longitude))
    {
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
    }
    else
    {
        painter.setPen(Qt::green);
        painter.setBrush(Qt::green);
    }
    painter.translate(w/2,h/2);
    painter.drawEllipse(s/-2,s/-2,s,s);
}

