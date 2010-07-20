#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
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
#include "qtrackdialog.h"

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG2( a ) std::cout << a
#define LOG( a )


QCurrentTrackTab::QCurrentTrackTab(QTrackTabsDialog* d, QTabWidget* t, Track* track)
    : QWidget(d), dialog(d), tab(t), center(0), time(0), dist(30)
{
	QVBoxLayout* main = new QVBoxLayout();
	QHBoxLayout* namebox = new QHBoxLayout();
	namebox->addWidget(new QLabel(tr("Name:")));
	trkname = new QLineEdit(this);
	
	if (track)
		SetTrackName(track->Name());
	else
		NewTrackName();
	
	namebox->addWidget(trkname);

	stop = new QPushButton(tr("Stop"));
	upd = new QPushButton(tr("Update"));
	start =  new QPushButton(tr("Start"));
	QPushButton* cancel =  new QPushButton(tr("Cancel"));
	
    QHBoxLayout* buttonbox =  new QHBoxLayout();
    buttonbox->addWidget(start);
    buttonbox->addWidget(stop);
    buttonbox->addWidget(upd);
    buttonbox->addWidget(cancel);
    
    tab->addTab(this,"");
    if (track)
		SwitchToCurrent();
    else
		SwitchToNew();

    QGroupBox *typegroup = new QGroupBox();
    QRadioButton *typeall  = new QRadioButton(tr("All"));
    QRadioButton *typetime = new QRadioButton(tr("Time based"));
    QRadioButton *typedist = new QRadioButton(tr("Distance based"));
    QVBoxLayout *typebox = new QVBoxLayout;
    typebox->addWidget(typeall);
    typebox->addWidget(typetime);
    typebox->addWidget(typedist);
    typebox->addStretch(1);
    typegroup->setLayout(typebox);

    distgroup = new QGroupBox();
    distbuttons = new QButtonGroup(distgroup);
    QRadioButton *dist10m  = new QRadioButton(tr("10m"));
    QRadioButton *dist30m  = new QRadioButton(tr("30m"));
    QRadioButton *dist100m = new QRadioButton(tr("100m"));
    QRadioButton *dist300m = new QRadioButton(tr("300m"));
    QRadioButton *dist1km  = new QRadioButton(tr("1km"));
    distbuttons->addButton(dist10m,  0);
    distbuttons->addButton(dist30m,  1);
    distbuttons->addButton(dist100m, 2);
    distbuttons->addButton(dist300m, 3);
    distbuttons->addButton(dist1km,  4);
    QVBoxLayout *distbox = new QVBoxLayout();
    distbox->addWidget(dist10m);
    distbox->addWidget(dist30m);
    distbox->addWidget(dist100m);
    distbox->addWidget(dist300m);
    distbox->addWidget(dist1km);
    distbox->addStretch(1);
    distgroup->setLayout(distbox);

    timegroup = new QGroupBox();
    timebuttons = new QButtonGroup(timegroup);
    QRadioButton *time5s  = new QRadioButton(tr( "5s"));
    QRadioButton *time15s = new QRadioButton(tr("15s"));
    QRadioButton *time1m  = new QRadioButton(tr( "1m"));
    QRadioButton *time5m  = new QRadioButton(tr( "5m"));
    QRadioButton *time15m = new QRadioButton(tr("15m"));
    timebuttons->addButton(time5s, 0);
    timebuttons->addButton(time15s,1);
    timebuttons->addButton(time1m, 2);
    timebuttons->addButton(time5m, 3);
    timebuttons->addButton(time15m,4);
    QVBoxLayout *timebox = new QVBoxLayout();
    timebox->addWidget(time5s);
    timebox->addWidget(time15s);
    timebox->addWidget(time1m);
    timebox->addWidget(time5m);
    timebox->addWidget(time15m);
    timebox->addStretch(1);
    timegroup->setLayout(timebox);

    QWidget *filler = new QWidget;
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    center = new QBoxLayout(QBoxLayout::LeftToRight);
    center->addWidget(typegroup);
    center->addWidget(distgroup);
    center->addWidget(timegroup);
    main->addLayout(namebox);
    main->addLayout(center);
    main->addWidget(filler);
    main->addLayout(buttonbox);
    setLayout(main);

    typedist->setChecked(true);
    dist30m->setChecked(true);
    time15s->setChecked(true);
    distgroup->show();
    timegroup->hide();

    connect(start,SIGNAL(clicked()),this,SLOT(emitnewtrack()));
    connect(stop,SIGNAL(clicked()),this,SLOT(emitstoptrack()));
    connect(upd,SIGNAL(clicked()),this,SLOT(emitupdatetrack()));
    connect(cancel,SIGNAL(clicked()),dialog,SLOT(reject()));
    
    
    connect(typeall, SIGNAL(clicked()),this,SLOT(selectall()));
    connect(typetime,SIGNAL(clicked()),this,SLOT(selecttime()));
    connect(typedist,SIGNAL(clicked()),this,SLOT(selectdistance()));
    connect(timebuttons, SIGNAL(buttonClicked(int)),this,SLOT(updatetime(int)));
    connect(distbuttons, SIGNAL(buttonClicked(int)),this,SLOT(updatedistance(int)));

    connect(this,SIGNAL(newtrack(QString,int,int)),   dialog,SIGNAL(newtrack(QString,int,int)));
    connect(this,SIGNAL(updatetrack(QString,int,int)),dialog,SIGNAL(updatetrack(QString,int,int)));
    connect(this,SIGNAL(stoptrack(QString)),          dialog,SIGNAL(stoptrack(QString)));
}

void QCurrentTrackTab::SetTrackName(QString name)
{
	trackname = name;
	trkname->setText(trackname);
}

void QCurrentTrackTab::NewTrackName()
{
	QDateTime curtime = QDateTime::currentDateTime().toUTC();
    SetTrackName(curtime.toString("trk-yyyyMMdd-hhmmss"));
}

void QCurrentTrackTab::SwitchToNew()
{
	NewTrackName();
	tab->setTabText(0,tr("New"));
    stop->hide();
    upd->hide();
    start->show();
    update();
}

void QCurrentTrackTab::SwitchToCurrent()
{
	tab->setTabText(0,tr("Current"));
    start->hide();
    stop->show();
    upd->show();
    update();
}

void QCurrentTrackTab::resizeEvent( QResizeEvent * event )
{
    LOG( "QCurrentTrackTab::resizeEvent()\n"; )
    if (!center) return;
    
    if (event->size().width() < event->size().height())
        center->setDirection(QBoxLayout::TopToBottom);
    else
        center->setDirection(QBoxLayout::LeftToRight);

    QWidget::resizeEvent(event);
}

void QCurrentTrackTab::selectall()
{
    time = 0;
    dist = 0;
    timegroup->hide();
    distgroup->hide();
}

void QCurrentTrackTab::selecttime()
{
    dist = 0;
    distgroup->hide();
    updatetime(timebuttons->checkedId());
    timegroup->show();
}

void QCurrentTrackTab::updatetime(int id)
{
    int values[5] = { 5, 15, 60, 300, 900 };
    time = values[id];
}

void QCurrentTrackTab::selectdistance()
{
    time = 0;
    timegroup->hide();
    updatedistance(distbuttons->checkedId());
    distgroup->show();
}

void QCurrentTrackTab::updatedistance(int id)
{
    int values[5] = { 10, 30, 100, 300, 1000 };
    dist = values[id];
}

QCurrentTrackTab::~QCurrentTrackTab()
{
}



QTrackListWidget::QTrackListWidget(QTrackListTab* parent)  
: QWidget(parent)
{
	center = new QVBoxLayout();
	togglemapper = new QSignalMapper(this);
	deletemapper = new QSignalMapper(this);

	QStringList files = TrackFiles();
	for (int i=0; i<files.length(); i++)
	{
		QHBoxLayout* item = new QHBoxLayout();
		QToolButton* togglebutton = new QToolButton();
		QToolButton* delbutton = new QToolButton();
		QWidget*     filler = new QWidget();
		delbutton->setIcon(QIcon(QPixmap(DASHRCDIR    "delete.svg")));
		filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		
		QStringList  keys = TrackList::Instance()->Keys();
		if (keys.contains(files[i]))
			togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "visible.svg")));
		else
			togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "invisible.svg")));
		
		item->addWidget(delbutton);
		item->addWidget(togglebutton);
		item->addWidget(new QLabel(files[i]));
		item->addWidget(filler);
		center->addLayout(item);
		items[files[i]]=item;
		
		connect(delbutton,SIGNAL(clicked()),deletemapper,SLOT(map()));
		connect(togglebutton,SIGNAL(clicked()),togglemapper,SLOT(map()));
		deletemapper->setMapping(delbutton,files[i]);
		togglemapper->setMapping(togglebutton,files[i]);
	}
	connect(deletemapper,SIGNAL(mapped(const QString&)),this,SLOT(DeleteTrack(const QString &)));
	connect(togglemapper,SIGNAL(mapped(const QString&)),this,SLOT(ToggleTrack(const QString &)));
	connect(TrackList::Instance(),SIGNAL(added(QString)),this,SLOT(TrackAdded(QString)));
	connect(TrackList::Instance(),SIGNAL(removed(QString)),this,SLOT(TrackRemoved(QString)));

	QWidget* filler = new QWidget();
	filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	center->addWidget(filler);
	
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setLayout(center);
}

void QTrackListWidget::TrackRemoved(QString name)
{
	LOG( "QTrackListTab::TrackRemoved(): " << name.toStdString() << "\n"; )
	QToolButton* togglebutton = (QToolButton*) togglemapper->mapping(name);
	togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "invisible.svg")));
}

void QTrackListWidget::TrackAdded(QString name)
{
	LOG( "QTrackListTab::TrackAdded(): " << name.toStdString() << "\n"; )
	QToolButton* togglebutton = (QToolButton*) togglemapper->mapping(name);
	togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "visible.svg")));
}

QStringList QTrackListWidget::TrackFiles()
{
	QDir directory = QDir(GetDrive() + QString(TRACKDIR));
	QStringList files = directory.entryList(QStringList(QString("*.gpx")),
															 QDir::Files | QDir::NoSymLinks);

	LOG( "QTrackListTab::TrackFiles() #gpx: " << files.size() << "\n"; )
	for (int i = 0; i < files.length(); ++i)
	{
			files[i] = files[i].left(files[i].length()-4);
	}
	return files;
}

void QTrackListWidget::DeleteTrack(const QString& name)
{
	LOG( "QTrackListWidget::DeleteTrack(): " << name.toStdString() << "\n"; )
    QHBoxLayout* item = items[name];
	center->removeItem(item);
	items.remove(name);
	delete item;
	emit deletetrack(name);
}

void QTrackListWidget::ToggleTrack(const QString& name)
{
	LOG( "QTrackListWidget::ToggleTrack(): " << name.toStdString() << "\n"; )
	QStringList  keys = TrackList::Instance()->Keys();
	if (keys.contains(name))
		emit hidetrack(name);
	else
		emit showtrack(name);
}



QTrackListTab::QTrackListTab(QTrackTabsDialog *parent)
: QWidget(parent)
{
    QVBoxLayout* main = new QVBoxLayout();
    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* exit = new QPushButton(tr("Cancel"));
    QWidget*     filler = new QWidget;
    QScrollArea* scroll = new QScrollArea();
    QTrackListWidget* list = new QTrackListWidget();
    
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    buttons->addWidget(exit);
    
    scroll->setWidget(list);
    scroll->show();
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    main->addWidget(scroll);
    main->addLayout(buttons);
    setLayout(main);

    exit->show();
    setAttribute(Qt::WA_DeleteOnClose);

    connect(list,SIGNAL(deletetrack(const QString&)),parent,SIGNAL(deletetrack(const QString&)));
    connect(list,SIGNAL(showtrack(const QString&)),  parent,SIGNAL(showtrack(const QString&)));
    connect(list,SIGNAL(hidetrack(const QString&)),  parent,SIGNAL(hidetrack(const QString&)));
    connect(exit,SIGNAL(clicked()),parent,SLOT(reject()));
}

QTrackListTab::~QTrackListTab()
{
}



QTrackTabsDialog::QTrackTabsDialog(Track* track, QWidget *parent)
    : QDialog(parent)
{
    QTabWidget* tabs = new QTabWidget(this);

    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);

    new QCurrentTrackTab(this,tabs,track);
    tabs->addTab(new QTrackListTab(this),tr("List"));
    tabs->addTab(new QWidget(this), tr("Options"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
    showFullScreen();
    setAttribute(Qt::WA_DeleteOnClose);
}

void QTrackTabsDialog::accept()
{
    QDialog::accept();
    close();
}
