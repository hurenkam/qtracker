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
#include <QSvgWidget>
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

//#include <QDebug>
//#define LOG( a ) qDebug() << a
#define LOG( a ) 

QCompassOptions::QCompassOptions(QDialog* d)
: QWidget(d)
, settings("karpeer.net","qTracker",this)
{
	source  = settings.value("compass/source",0).toInt();
	view    = settings.value("compass/view",0).toInt();
	montype = settings.value("monitor/type",0).toInt();
	wptname = settings.value("monitor/waypoint","").toString();
	rtename = settings.value("monitor/route","").toString();
	trkname = settings.value("monitor/track","").toString();
	
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
	sourcegroup->show();
	sensorbutton->setChecked(true);
	if (source==0)
	    sensorbutton->setChecked(true);
	else
	    gpsbutton->setChecked(true);
	
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
	viewgroup->show();
	headupbutton->setChecked(true);
	if (view==0)
	    northupbutton->setChecked(true);
	else
	    headupbutton->setChecked(true);
	
	typegroup = new QGroupBox("Monitor");
	typebuttons = new QButtonGroup(typegroup);
	QRadioButton* none =     new QRadioButton(tr("None"));
	QRadioButton* waypoint = new QRadioButton(tr("Waypoint"));
	QRadioButton* route =    new QRadioButton(tr("Route"));
	QRadioButton* track =    new QRadioButton(tr("Track"));
	typebuttons->addButton(none,0);
	typebuttons->addButton(waypoint, 1);
	typebuttons->addButton(route,    2);
	typebuttons->addButton(track,    3);
	QVBoxLayout *typebox = new QVBoxLayout();
	typebox->addWidget(none);
	typebox->addWidget(waypoint);
	typebox->addWidget(route);
	typebox->addWidget(track);
	typegroup->setLayout(typebox);
	typegroup->show();

	filler = new QWidget;
	filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	QStringList wptlist = WayPointList::Instance().Keys();
	wptcombo = new QComboBox();
	wptcombo->addItems(wptlist);
    if ((wptlist.length()>0) && (wptlist.contains(wptname)))
        wptcombo->setCurrentIndex(wptlist.indexOf(wptname));
    
	QStringList rtelist = RouteList::Instance()->Keys();
	rtecombo = new QComboBox();
	rtecombo->addItems(rtelist);
    if ((rtelist.length()>0) && (rtelist.contains(wptname)))
        rtecombo->setCurrentIndex(rtelist.indexOf(wptname));
	
	QStringList trklist = TrackList::Instance()->Keys();
	trkcombo = new QComboBox();
	trkcombo->addItems(trklist);
    if ((trklist.length()>0) && (trklist.contains(trkname)))
        trkcombo->setCurrentIndex(trklist.indexOf(trkname));
	
	switch (montype)
	{
		default:
		case 0:
			none->setChecked(true);
			noneselected();
			break;
		case 1: 
			waypoint->setChecked(true);
			waypointselected();
			break;
		case 2: 
			route->setChecked(true);
			routeselected();
			break;
		case 3: 
			track->setChecked(true);
			trackselected();
			break;
	}

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
	bottomright->addWidget(typegroup);
	bottomright->addWidget(wptcombo);
	bottomright->addWidget(rtecombo);
	bottomright->addWidget(trkcombo);
	//main->addWidget(filler);
	center->addLayout(topleft);
	center->addLayout(bottomright);
	main->addLayout(center);
	main->addLayout(buttonbox);

    connect(none,    SIGNAL(clicked()),this,SLOT(noneselected()));
    connect(waypoint,SIGNAL(clicked()),this,SLOT(waypointselected()));
    connect(route,   SIGNAL(clicked()),this,SLOT(routeselected()));
    connect(track,   SIGNAL(clicked()),this,SLOT(trackselected()));

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
	montype = typebuttons->checkedId();
	switch (montype)
	{
		case 1:
			wptname = wptcombo->currentText();
			break;
		case 2:
			rtename = rtecombo->currentText();
			break;
		case 3:
			trkname = trkcombo->currentText();
			break;
		default:
			break;
	}
	settings.setValue("compass/source",source);
	settings.setValue("compass/view",view);
	settings.setValue("monitor/type",montype);
	settings.setValue("monitor/waypoint",wptname);
	settings.setValue("monitor/route",rtename);
	settings.setValue("monitor/track",trkname);
	settings.sync();
	emit changed();
}

void QCompassOptions::noneselected()
{
    LOG( "QCompassOptions::noneselected()"; )
	wptcombo->hide();
	rtecombo->hide();
	trkcombo->hide();
	update();
}

void QCompassOptions::waypointselected()
{
    LOG( "QCompassOptions::waypointselected()"; )
	wptcombo->show();
	rtecombo->hide();
	trkcombo->hide();
	update();
}

void QCompassOptions::routeselected()
{
    LOG( "QCompassOptions::routeselected()"; )
	wptcombo->hide();
	rtecombo->show();
	trkcombo->hide();
	update();
}

void QCompassOptions::trackselected()
{
    LOG( "QCompassOptions::trackselected()"; )
	wptcombo->hide();
	rtecombo->hide();
	trkcombo->show();
	update();
}

QCompassDialog::QCompassDialog(QHeadingWidget *parent)
    : QDialog(parent)
{
    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);

    QTabWidget* tabs = new QTabWidget(this);
    QCompassOptions* options = new QCompassOptions(this);
    tabs->addTab(options, tr("Compass"));
     
    QVBoxLayout *main = new QVBoxLayout;
    main->addWidget(tabs);

	connect(options, SIGNAL(changed()),this,SLOT(accept()));
    
    setLayout(main);
    showFullScreen();
    setAttribute(Qt::WA_DeleteOnClose);
}
