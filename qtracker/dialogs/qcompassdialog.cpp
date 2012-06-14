#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QResizeEvent>
#include <QDateTime>
#include <QToolButton>
#include <QIcon>
#include <QTabWidget>
#include <QFile>
#include <QListWidget>
#include "qcompassdialog.h"
#include "qheadingwidget.h"
#include "ui.h"
#include "waypointlist.h"
#include "tracklist.h"
#include "routelist.h"
#include "datamonitor.h"

//#include <QDebug>
//#define LOG( a ) qDebug() << a
#define LOG( a ) 

QCompassOptions::QCompassOptions(QDialog* d)
: QWidget(d)
, settings("karpeer.net","qTracker",this)
{
    source  = settings.value("compass/source",0).toInt();
    view    = settings.value("compass/view",0).toInt();

    QVBoxLayout *main = new QVBoxLayout();

    QGroupBox*    sourcegroup =   new QGroupBox("Source");
    sourcebuttons = new QButtonGroup(sourcegroup);
    QRadioButton* sensorbutton =  new QRadioButton("Sensor");
    QRadioButton* gpsbutton =     new QRadioButton("GPS");
    QVBoxLayout*  sourcebox =     new QVBoxLayout();
    sourcebuttons->addButton(sensorbutton,0);
    sourcebuttons->addButton(gpsbutton,1);
    sourcebox->addWidget(sensorbutton);
    sourcebox->addWidget(gpsbutton);
    sourcegroup->setLayout(sourcebox);
    //sourcegroup->show();

    if ( DataMonitor::Instance().IsUsingGPSCompass() )
    gpsbutton->setChecked(true);
    else
    sensorbutton->setChecked(true);

    if ( !DataMonitor::Instance().HasCompassSensor() )
            sensorbutton->setDisabled(true);

    QGroupBox*    viewgroup =     new QGroupBox("View");
    viewbuttons =   new QButtonGroup(viewgroup);
    QRadioButton* northupbutton = new QRadioButton("North up");
    QRadioButton* headupbutton =  new QRadioButton("Heading up");
    QVBoxLayout*  viewbox =       new QVBoxLayout();
    viewbuttons->addButton(northupbutton,0);
    viewbuttons->addButton(headupbutton,1);
    viewbox->addWidget(northupbutton);
    viewbox->addWidget(headupbutton);
    viewgroup->setLayout(viewbox);
    //viewgroup->show();
    headupbutton->setChecked(true);
    if (view==0)
        northupbutton->setChecked(true);
    else
        headupbutton->setChecked(true);

    filler = new QWidget;
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPushButton* apply =  new QPushButton(tr("Apply"));
    QPushButton* cancel =  new QPushButton(tr("Cancel"));
    QHBoxLayout* buttonbox =  new QHBoxLayout();
    buttonbox->addWidget(apply);
    buttonbox->addWidget(cancel);

    QVBoxLayout* topleft = new QVBoxLayout();
    QVBoxLayout* bottomright = new QVBoxLayout();
    center = new QBoxLayout(QBoxLayout::TopToBottom);
	
    topleft->addWidget(sourcegroup);
    topleft->addWidget(viewgroup);
    bottomright->addWidget(filler);
    center->addLayout(topleft);
    center->addLayout(bottomright);
    main->addLayout(center);
    main->addLayout(buttonbox);

    connect(apply,   SIGNAL(clicked()),this,SLOT(apply()));
    connect(cancel,  SIGNAL(clicked()),d,SLOT(reject()));
    setLayout(main);
}

QCompassOptions::~QCompassOptions()
{
}

void QCompassOptions::resizeEvent( QResizeEvent * event )
{
    LOG( "QCompassOptions::resizeEvent()\n"; )
    if (!center) return;
    
    if (event->size().width() < event->size().height())
        center->setDirection(QBoxLayout::TopToBottom);
    else
        center->setDirection(QBoxLayout::LeftToRight);

    QWidget::resizeEvent(event);
}


void QCompassOptions::apply()
{
    source  = sourcebuttons->checkedId();
    view    = viewbuttons->checkedId();

    settings.setValue("compass/source",source);
    settings.setValue("compass/view",view);

    settings.sync();
    emit changed();
}

QCompassDialog::QCompassDialog(QHeadingWidget *parent)
    //: QDialog(parent)
{
    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);

    QTabWidget* tabs = new QTabWidget(this);
    QCompassOptions* options = new QCompassOptions(this);
    QMonitorOptions* monitor = new QMonitorOptions(this);
    tabs->addTab(options, tr("Compass"));
    tabs->addTab(monitor, tr("Monitor"));
     
    QVBoxLayout *main = new QVBoxLayout;
    main->addWidget(tabs);

    connect(options, SIGNAL(changed()),this,SLOT(accept()));
    connect(monitor, SIGNAL(changed()),this,SLOT(accept()));
    
    setLayout(main);
    move(0,0);
    setModal(true);
    showFullScreen();
    setAttribute(Qt::WA_DeleteOnClose);
}
