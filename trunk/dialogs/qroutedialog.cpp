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
#include "qroutedialog.h"

#include <QDebug>
#define LOG( a )  qDebug() << a
#define LOG2( a ) 
#define LOG3( a ) 


QCurrentRouteTab::QCurrentRouteTab(QRouteTabsDialog* d, QTabWidget* t, Route* route)
    : QWidget(d), dialog(d), tab(t), time(0), dist(30), center(0)
{
	QVBoxLayout* main = new QVBoxLayout();
	QHBoxLayout* namebox = new QHBoxLayout();
	namebox->addWidget(new QLabel(tr("Name:")));
	trkname = new QLineEdit(this);
	
	if (route)
		SetRouteName(route->Name());
	else
		NewRouteName();
	
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
    if (route)
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

    connect(start,SIGNAL(clicked()),this,SLOT(emitnewroute()));
    connect(stop,SIGNAL(clicked()),this,SLOT(emitstoproute()));
    connect(upd,SIGNAL(clicked()),this,SLOT(emitupdateroute()));
    connect(cancel,SIGNAL(clicked()),dialog,SLOT(reject()));
    
    
    connect(typeall, SIGNAL(clicked()),this,SLOT(selectall()));
    connect(typetime,SIGNAL(clicked()),this,SLOT(selecttime()));
    connect(typedist,SIGNAL(clicked()),this,SLOT(selectdistance()));
    connect(timebuttons, SIGNAL(buttonClicked(int)),this,SLOT(updatetime(int)));
    connect(distbuttons, SIGNAL(buttonClicked(int)),this,SLOT(updatedistance(int)));
}

void QCurrentRouteTab::SetRouteName(QString name)
{
	routename = name;
	trkname->setText(routename);
}

void QCurrentRouteTab::NewRouteName()
{
	QDateTime curtime = QDateTime::currentDateTime().toUTC();
    SetRouteName(curtime.toString("trk-yyyyMMdd-hhmmss"));
}

void QCurrentRouteTab::SwitchToNew()
{
	NewRouteName();
	tab->setTabText(0,tr("New"));
    stop->hide();
    upd->hide();
    start->show();
    update();
}

void QCurrentRouteTab::SwitchToCurrent()
{
	tab->setTabText(0,tr("Current"));
    start->hide();
    stop->show();
    upd->show();
    update();
}

void QCurrentRouteTab::resizeEvent( QResizeEvent * event )
{
    LOG( "QCurrentRouteTab::resizeEvent()"; )
    if (!center) return;
    
    if (event->size().width() < event->size().height())
        center->setDirection(QBoxLayout::TopToBottom);
    else
        center->setDirection(QBoxLayout::LeftToRight);

    QWidget::resizeEvent(event);
}

void QCurrentRouteTab::selectall()
{
    time = 0;
    dist = 0;
    timegroup->hide();
    distgroup->hide();
}

void QCurrentRouteTab::selecttime()
{
    dist = 0;
    distgroup->hide();
    updatetime(timebuttons->checkedId());
    timegroup->show();
}

void QCurrentRouteTab::updatetime(int id)
{
    int values[5] = { 5, 15, 60, 300, 900 };
    time = values[id];
}

void QCurrentRouteTab::selectdistance()
{
    time = 0;
    timegroup->hide();
    updatedistance(distbuttons->checkedId());
    distgroup->show();
}

void QCurrentRouteTab::updatedistance(int id)
{
    int values[5] = { 10, 30, 100, 300, 1000 };
    dist = values[id];
}

QCurrentRouteTab::~QCurrentRouteTab()
{
}


QRouteListWidget::QRouteListWidget(QRouteListTab* parent)  
: QWidget(parent)
{
	center = new QVBoxLayout();
	togglemapper = new QSignalMapper(this);
	deletemapper = new QSignalMapper(this);

	QStringList allkeys = RouteList::Instance()->Keys();
	QStringList visiblekeys = RouteList::Instance()->VisibleKeys();
	for (int i=0; i<allkeys.length(); i++)
		NewItem(allkeys[i],visiblekeys.contains(allkeys[i]));

	connect(deletemapper,SIGNAL(mapped(const QString&)),this,SLOT(DeleteRoute(const QString &)));
	connect(togglemapper,SIGNAL(mapped(const QString&)),this,SLOT(ToggleRoute(const QString &)));
	connect(RouteList::Instance(),SIGNAL(visible(const QString&)),this,SLOT(RouteVisible(const QString&)));
	connect(RouteList::Instance(),SIGNAL(invisible(const QString&)),this,SLOT(RouteInvisible(const QString&)));
	connect(RouteList::Instance(),SIGNAL(deleted(const QString&)),this,SLOT(RouteDeleted(const QString&)));
	connect(RouteList::Instance(),SIGNAL(added(const QString&)),this,SLOT(RouteAdded(const QString&)));

	QWidget* filler = new QWidget();
	filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	center->addWidget(filler);
	
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setLayout(center);
}

QHBoxLayout* QRouteListWidget::NewItem(const QString& name, bool visible)
{
	QHBoxLayout* item = new QHBoxLayout();
	QToolButton* togglebutton = new QToolButton();
	QToolButton* delbutton = new QToolButton();
	QWidget*     filler = new QWidget();
	delbutton->setIcon(QIcon(QPixmap(DASHRCDIR    "delete.svg")));
	filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	if (visible)
		togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "visible.svg")));
	else
		togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "invisible.svg")));
	
	item->addWidget(delbutton);
	item->addWidget(togglebutton);
	item->addWidget(new QLabel(name));
	item->addWidget(filler);
	center->addLayout(item);
	items[name]=item;
	
	connect(delbutton,SIGNAL(clicked()),deletemapper,SLOT(map()));
	connect(togglebutton,SIGNAL(clicked()),togglemapper,SLOT(map()));
	deletemapper->setMapping(delbutton,name);
	togglemapper->setMapping(togglebutton,name);
}

void QRouteListWidget::DeleteRoute(const QString& name)
{
	LOG( "QRouteListWidget::DeleteRoute(): " << name; )
    RouteList::Instance()->Delete(name);
}

void QRouteListWidget::ToggleRoute(const QString& name)
{
	LOG( "QRouteListWidget::ToggleRoute(): " << name; )
	QStringList  keys = RouteList::Instance()->VisibleKeys();
	if (keys.contains(name))
		RouteList::Instance()->Hide(name);
	else
		RouteList::Instance()->Show(name);
}

void QRouteListWidget::RouteInvisible(const QString& name)
{
	LOG( "QRouteListTab::RouteRemoved(): " << name; )
	QToolButton* togglebutton = (QToolButton*) togglemapper->mapping(name);
	if (togglebutton)
	    togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "invisible.svg")));
}

void QRouteListWidget::RouteVisible(const QString& name)
{
	LOG( "QRouteListTab::RouteAdded(): " << name; )
	QToolButton* togglebutton = (QToolButton*) togglemapper->mapping(name);
	if (togglebutton)
	    togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "visible.svg")));
}

void QRouteListWidget::RouteDeleted(const QString& name)
{
	QHBoxLayout* item = items[name];
	if (!item) return;
	
	center->removeItem(item);
	items.remove(name);
	delete item;
	updateGeometry();
}

void QRouteListWidget::RouteAdded(const QString& name)
{
	QStringList visiblekeys = RouteList::Instance()->VisibleKeys();
    NewItem(name,visiblekeys.contains(name));
	updateGeometry();
}



QRouteListTab::QRouteListTab(QRouteTabsDialog *parent)
: QWidget(parent)
{
    QVBoxLayout* main = new QVBoxLayout();
    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* exit = new QPushButton(tr("Cancel"));
    QWidget*     filler = new QWidget;
    QScrollArea* scroll = new QScrollArea();
    QRouteListWidget* list = new QRouteListWidget();
    
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

    connect(exit,SIGNAL(clicked()),parent,SLOT(reject()));
}

QRouteListTab::~QRouteListTab()
{
}



QRouteTabsDialog::QRouteTabsDialog(Route* route, QWidget *parent)
    : QDialog(parent)
{
    QTabWidget* tabs = new QTabWidget(this);

    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);

    tabs->addTab(new QRouteListTab(this),tr("List"));
    tabs->addTab(new QWidget(this), tr("Options"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
    showFullScreen();
    setAttribute(Qt::WA_DeleteOnClose);
}

void QRouteTabsDialog::accept()
{
    QDialog::accept();
    close();
}
