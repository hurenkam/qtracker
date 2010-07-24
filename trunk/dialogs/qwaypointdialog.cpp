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
#include <QFile>
#include "qwaypointdialog.h"
#include "ui.h"
#include "geodata.h"

#include <QDebug>
#define LOG( a ) qDebug() << a
#define LOG2( a ) 



QWayPointTabsDialog::QWayPointTabsDialog(const WayPoint& w, QWidget *p)
{
    QTabWidget* tabs = new QTabWidget(this);

    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);

    new QEditWayPointTab(this,tabs,w);
    tabs->addTab(new QWayPointListTab(this),tr("List"));
    tabs->addTab(new QWidget(this), tr("Options"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
    showFullScreen();
    setAttribute(Qt::WA_DeleteOnClose);
}

void QWayPointTabsDialog::accept()
{
    QDialog::accept();
    close();
}



QEditWayPointTab::QEditWayPointTab(QWayPointTabsDialog *d, QTabWidget* t, const WayPoint& w)
: QWidget(d), dialog(d), tab(t), name(0), time(0), latitude(0), longitude(0), elevation(0)
{
	QVBoxLayout* main = new QVBoxLayout();
	QGridLayout* gridbox = new QGridLayout();
	
	// Name ======================================
	name = new QLineEdit(this);
	
	QString prefix = w.Name();
	if (prefix!="<new>")
	{
		name->setText(prefix);
		tab->addTab(this,"Edit");
	}
	else
	{
		name->setText(UniqueName("wpt"));
        tab->addTab(this,"New" );
	}
	gridbox->addWidget(new QLabel(tr("Name:")),0,0);
	gridbox->addWidget(name,0,1);

	// Latitude ======================================
    latitude = new QDoubleEdit(w.Latitude(),this);
	gridbox->addWidget(new QLabel(tr("Latitude:")),1,0);
    gridbox->addWidget(latitude,1,1);
    
	// Longitude ======================================
    longitude = new QDoubleEdit(w.Longitude(),this);
	gridbox->addWidget(new QLabel(tr("Longitude:")),2,0);
    gridbox->addWidget(longitude,2,1);
    
	// Elevation ======================================
    elevation = new QDoubleEdit(w.Elevation(),this);
	gridbox->addWidget(new QLabel(tr("Altitude:")),3,0);
    gridbox->addWidget(elevation,3,1);
    
	// Time ======================================
    time = new QLineEdit(w.Time(),this);
	gridbox->addWidget(new QLabel(tr("Time:")),4,0);
    gridbox->addWidget(time,4,1);
    
	// Filler ======================================
    QWidget *filler = new QWidget;
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
	// Buttons ======================================
	QPushButton* confirm =  new QPushButton(tr("Save"));
	QPushButton* cancel  =  new QPushButton(tr("Cancel"));
	QHBoxLayout* buttonbox =  new QHBoxLayout();
	buttonbox->addWidget(confirm);
	buttonbox->addWidget(cancel);

	// Layout ======================================
    main->addLayout(gridbox);
    main->addWidget(filler);
    main->addLayout(buttonbox);
    setLayout(main);

	// Signals ======================================
    connect(cancel,SIGNAL(clicked()),d,SLOT(reject()));
    connect(confirm,SIGNAL(clicked()),this,SLOT(accept()));
    connect(confirm,SIGNAL(clicked()),d,SLOT(accept()));
}

QEditWayPointTab::~QEditWayPointTab()
{
}

void QEditWayPointTab::accept() 
{
    WayPoint w = Position();
    WayPointList::Instance().AddWayPoint(w);
}

void QEditWayPointTab::setvalue(const WayPoint& w)
{
    name->setText(w.Name());
    time->setText(w.Time());
    latitude->setNumber(w.Latitude());
    longitude->setNumber(w.Longitude());
    elevation->setNumber(w.Elevation());
}


QWayPointListWidget::QWayPointListWidget(QWayPointListTab* parent)  
: QWidget(parent)
{
	center = new QVBoxLayout();
	togglemapper = new QSignalMapper(this);
	editmapper = new QSignalMapper(this);
	deletemapper = new QSignalMapper(this);

	QStringList names = WayPointList::Instance().Keys();
	QStringList visible = WayPointList::Instance().VisibleKeys();
	for (int i=0; i<names.length(); i++)
	{
		QHBoxLayout* item = new QHBoxLayout();
		QToolButton* togglebutton = new QToolButton();
		QToolButton* delbutton = new QToolButton();
		QToolButton* editbutton = new QToolButton();
		QWidget*     filler = new QWidget();
		delbutton->setIcon(QIcon(QPixmap(DASHRCDIR    "delete.svg")));
		editbutton->setIcon(QIcon(QPixmap(DASHRCDIR   "edit.svg")));
		if (visible.contains(names[i]))
            togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "visible.svg")));
		else
            togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "invisible.svg")));
		filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		
		item->addWidget(delbutton);
		item->addWidget(editbutton);
		item->addWidget(togglebutton);
		item->addWidget(new QLabel(names[i]));
		item->addWidget(filler);
		center->addLayout(item);
		items[names[i]]=item;
		
		connect(delbutton,SIGNAL(clicked()),deletemapper,SLOT(map()));
		connect(editbutton,SIGNAL(clicked()),editmapper,SLOT(map()));
		connect(togglebutton,SIGNAL(clicked()),togglemapper,SLOT(map()));
		deletemapper->setMapping(delbutton,names[i]);
		editmapper->setMapping(editbutton,names[i]);
		togglemapper->setMapping(togglebutton,names[i]);
	}
	connect(deletemapper,SIGNAL(mapped(const QString&)),this,SLOT(DeleteWayPoint(const QString &)));
	connect(editmapper,SIGNAL(mapped(const QString&)),this,SLOT(EditWayPoint(const QString &)));
	connect(togglemapper,SIGNAL(mapped(const QString&)),this,SLOT(ToggleWayPoint(const QString &)));
	connect(&WayPointList::Instance(),SIGNAL(invisible(const QString&)),this,SLOT(WayPointHidden(const QString&)));
	connect(&WayPointList::Instance(),SIGNAL(visible(const QString&)),this,SLOT(WayPointVisible(const QString&)));

	QWidget* filler = new QWidget();
	filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	center->addWidget(filler);
	
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setLayout(center);
}

void QWayPointListWidget::WayPointHidden(const QString& name)
{
	LOG( "QWayPointListTab::WayPointHidden(): " << name; )
	QToolButton* togglebutton = (QToolButton*) togglemapper->mapping(name);
	if (togglebutton)
	    togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "invisible.svg")));
}

void QWayPointListWidget::WayPointVisible(const QString& name)
{
	LOG( "QWayPointListWidget::WayPointVisible(): " << name; )
	QToolButton* togglebutton = (QToolButton*) togglemapper->mapping(name);
	if (togglebutton)
	    togglebutton->setIcon(QIcon(QPixmap(DASHRCDIR "visible.svg")));
}

void QWayPointListWidget::DeleteWayPoint(const QString& name)
{
	LOG( "QWayPointListWidget::DeleteWayPoint(): " << name; )
    QHBoxLayout* item = items[name];
	center->removeItem(item);
	items.remove(name);
	delete item;
	//emit deletewaypoint(name);
	WayPointList::Instance().RemoveWayPoint(name);
}

void QWayPointListWidget::EditWayPoint(const QString& name)
{
	LOG( "QWayPointListWidget::EditWayPoint(): " << name; )
}

void QWayPointListWidget::ToggleWayPoint(const QString& name)
{
	LOG( "QWayPointListWidget::ToggleWayPoint(): " << name; )
	if (WayPointList::Instance().IsVisible(name))
		WayPointList::Instance().Hide(name);
	else
		WayPointList::Instance().Show(name);
}



QWayPointListTab::QWayPointListTab(QWayPointTabsDialog *parent)
: QWidget(parent)
{
    QVBoxLayout* main = new QVBoxLayout();
    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* exit = new QPushButton(tr("Cancel"));
    QWidget*     filler = new QWidget;
    QScrollArea* scroll = new QScrollArea();
    QWayPointListWidget* list = new QWayPointListWidget();
    
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

    connect(list,SIGNAL(deletewaypoint(const QString&)),parent,SIGNAL(deletewaypoint(const QString&)));
    connect(list,SIGNAL(showwaypoint(const QString&)),  parent,SIGNAL(showwaypoint(const QString&)));
    connect(list,SIGNAL(hidewaypoint(const QString&)),  parent,SIGNAL(hidewaypoint(const QString&)));
    connect(exit,SIGNAL(clicked()),parent,SLOT(reject()));
}

QWayPointListTab::~QWayPointListTab()
{
}
