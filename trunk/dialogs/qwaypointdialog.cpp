#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QResizeEvent>
#include "qwaypointdialog.h"

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG2( a ) std::cout << a
#define LOG( a )


QWaypointDialog::QWaypointDialog(QString title, QString name, double lat, double lon, QWidget *parent)
    : QDialog(parent)
{
    mainbox =  new QVBoxLayout();
    gridbox = new QGridLayout();
    groupbox = new QGroupBox(title);
    groupbox->setLayout(gridbox);
    buttons =  new QHBoxLayout();
    wptname = new QLineEdit(name,this);
    latitude = new QDoubleEdit(lat,this);
    longitude = new QDoubleEdit(lon,this);

    cancel =  new QPushButton(tr("Cancel"));
    confirm = new QPushButton(tr("Confirm"));

    gridbox->addWidget(new QLabel(tr("Name:")),0,0);
    gridbox->addWidget(new QLabel(tr("Latitude:")),1,0);
    gridbox->addWidget(new QLabel(tr("Longitude:")),2,0);
    gridbox->addWidget(wptname,0,1);
    gridbox->addWidget(latitude,1,1);
    gridbox->addWidget(longitude,2,1);

    buttons->addWidget(confirm);
    buttons->addWidget(cancel);
    mainbox->addWidget(groupbox);
    mainbox->addLayout(buttons);
    setLayout(mainbox);

    cancel->show();
    confirm->show();
    setAttribute(Qt::WA_DeleteOnClose);

    connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(confirm,SIGNAL(clicked()),this,SLOT(accept()));
    //connect(this,SIGNAL(accepted),this,SLOT(emitSelection()));
}

void QWaypointDialog::accept()
{
    QString name = wptname->text();
    double lat = latitude->text().toDouble();
    double lon = longitude->text().toDouble();
    emit confirmed(name,lat,lon);
    QDialog::accept();
    close();
}

QWaypointDialog::~QWaypointDialog()
{
    delete wptname;
    delete latitude;
    delete longitude;
    delete cancel;
    delete confirm;
    delete buttons;
    delete gridbox;
    delete mainbox;
}

QNewTrackTab::QNewTrackTab(QTrackTabsDialog *parent)
    : QWidget(parent), center(0)
{
	trkname = new QLineEdit("trk",this);
	
	main = new QVBoxLayout();
	
	QHBoxLayout *namebox = new QHBoxLayout();
	namebox->addWidget(new QLabel(tr("Name:")));
	namebox->addWidget(trkname);

	QHBoxLayout *buttonbox =  new QHBoxLayout();
	QPushButton *cancel =  new QPushButton(tr("Cancel"));
	QPushButton *confirm = new QPushButton(tr("Start"));
	buttonbox->addWidget(confirm);
	buttonbox->addWidget(cancel);
	
	QGroupBox *typegroup = new QGroupBox();
	QRadioButton *type1 = new QRadioButton(tr("&All"));
	QRadioButton *type2 = new QRadioButton(tr("&Time based"));
	QRadioButton *type3 = new QRadioButton(tr("&Distance based"));
	type3->setChecked(true);
	QVBoxLayout *typebox = new QVBoxLayout;
	typebox->addWidget(type1);
	typebox->addWidget(type2);
	typebox->addWidget(type3);
	typebox->addStretch(1);
	typegroup->setLayout(typebox);
	
	QGroupBox *distgroup = new QGroupBox();
	QRadioButton *dist1 = new QRadioButton(tr("10m"));
	QRadioButton *dist2 = new QRadioButton(tr("30m"));
	QRadioButton *dist3 = new QRadioButton(tr("100m"));
	QRadioButton *dist4 = new QRadioButton(tr("300m"));
	QRadioButton *dist5 = new QRadioButton(tr("1km"));
	dist2->setChecked(true);
	QVBoxLayout *distbox = new QVBoxLayout;
	distbox->addWidget(dist1);
	distbox->addWidget(dist2);
	distbox->addWidget(dist3);
	distbox->addWidget(dist4);
	distbox->addWidget(dist5);
	distbox->addStretch(1);
	distgroup->setLayout(distbox);
	
    QWidget *filler = new QWidget;
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	center = new QBoxLayout(QBoxLayout::LeftToRight);
	center->addWidget(typegroup);
	center->addWidget(distgroup);
	main->addLayout(namebox);
	main->addLayout(center);
	main->addWidget(filler);
	main->addLayout(buttonbox);
	setLayout(main);
	
	cancel->show();
	confirm->show();
	
	connect(cancel,SIGNAL(clicked()),parent,SLOT(reject()));
	connect(confirm,SIGNAL(clicked()),this,SLOT(emitnewtrack()));
	connect(this,SIGNAL(newtrack(QString,int,int)),parent,SLOT(emitnewtrack(QString,int,int)));
}

void QNewTrackTab::resizeEvent( QResizeEvent * event )
{
    LOG( "QNewTrackTab::resizeEvent()\n"; )
    if (!center) return;
    
    if (event->size().width() < event->size().height())
	    center->setDirection(QBoxLayout::TopToBottom);
	else
	    center->setDirection(QBoxLayout::LeftToRight);

    QWidget::resizeEvent(event);
}

QNewTrackTab::~QNewTrackTab()
{
    delete trkname;
}

QTrackTabsDialog::QTrackTabsDialog(QString title, QString name, QWidget *parent)
    : QDialog(parent)
{
    tabs = new QTabWidget(this);
    //tabs->setStyleSheet("QTabBar::tab { border:1px; margin-top:1px; margin-bottom:1px }");
    tabs->addTab(new QNewTrackTab(this), tr("New"));
    tabs->addTab(new QNewTrackTab(this), tr("List"));
    tabs->addTab(new QNewTrackTab(this), tr("Options"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
    setAttribute(Qt::WA_DeleteOnClose);
}

void QTrackTabsDialog::emitnewtrack(QString name, int time, int distance)        
{ 
    LOG( "QTrackTabsDialog::emitnewtrack()\n"; )
	emit newtrack(name,time,distance);           
    accept();
}

void QTrackTabsDialog::emitdeletetrack(QString name)                             
{ 
    LOG2( "QTrackTabsDialog::emitdeletetrack()\n"; )
	emit deletetrack(name);                      
    accept();
}

void QTrackTabsDialog::emitchangeoptions(bool autostart, int time, int distance) 
{ 
    LOG2( "QTrackTabsDialog::emitchangeoptions()\n"; )
	emit changeoptions(autostart,time,distance); 
    accept();
}

void QTrackTabsDialog::accept()
{
    QDialog::accept();
    close();
}

QTrackTabsDialog::~QTrackTabsDialog()
{
    LOG( "QTrackTabsDialog::~QTrackTabsDialog()\n"; )
    delete tabs;
}


