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
#include <QList>
#include <QListWidgetItem>
#include "qmapdialog.h"
#include "maplist.h"
#include "gpxio.h"
#include "geocoords.hpp"

#include <QDebug>
#define LOG( a ) qDebug() << a
#define LOG2( a ) 

using namespace geodata;

QCurrentMapTab::QCurrentMapTab(QMapTabsDialog* d, QTabWidget* t)
    : QWidget(d), dialog(d), tab(t), center(0)
{
	QVBoxLayout* main = new QVBoxLayout();
	QHBoxLayout* namebox = new QHBoxLayout();
	namebox->addWidget(new QLabel(tr("Name:")));
	name = new QLineEdit(this);
	namebox->addWidget(name);
	QPushButton* cancel =  new QPushButton(tr("Cancel"));
    QHBoxLayout* buttonbox =  new QHBoxLayout();
    buttonbox->addWidget(cancel);
    tab->addTab(this,"Map");

    
    QWidget *filler = new QWidget;
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    center = new QBoxLayout(QBoxLayout::LeftToRight);
    main->addLayout(namebox);
    main->addLayout(center);
    main->addWidget(filler);
    main->addLayout(buttonbox);
    setLayout(main);

    connect(cancel,SIGNAL(clicked()),dialog,SLOT(reject()));
    connect(this,SIGNAL(updatemap(QString)),dialog,SIGNAL(updatemap(QString)));
}

void QCurrentMapTab::resizeEvent( QResizeEvent * event )
{
    LOG( "QCurrentTrackTab::resizeEvent()"; )
    if (!center) return;
    
    if (event->size().width() < event->size().height())
        center->setDirection(QBoxLayout::TopToBottom);
    else
        center->setDirection(QBoxLayout::LeftToRight);

    QWidget::resizeEvent(event);
}

QCurrentMapTab::~QCurrentMapTab()
{
}



QMapListTab::QMapListTab(QMapTabsDialog *d, int tabtype, WayPoint* p)
: QWidget(d), dialog(d)
{
    QVBoxLayout* main = new QVBoxLayout();
    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* confirm = new QPushButton(tr("Load"));
    QPushButton* exit = new QPushButton(tr("Cancel"));
    QWidget*     filler = new QWidget;
    list = new QListWidget();

    QStringList files;
    if (p)
    	files = MapList::Instance().FindMapsForPosition(*p);
    else
    	files = MapList::Instance().MapNames();
    
    for (int i = 0; i < files.size(); ++i)
    {
        new QListWidgetItem(files[i], list);
    }
    
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    buttons->addWidget(confirm);
    buttons->addWidget(exit);
    main->addWidget(list);
    main->addLayout(buttons);
    setLayout(main);

    exit->show();
    setAttribute(Qt::WA_DeleteOnClose);

    connect(confirm,SIGNAL(clicked()),this,SLOT(confirmselection()));
    connect(exit,SIGNAL(clicked()),dialog,SLOT(reject()));
    connect(this,SIGNAL(mapselected(QString)),dialog,SIGNAL(loadmap(QString)));
}

void QMapListTab::confirmselection()
{
    LOG( "QMapListTab::confirmselection()"; )
    QString filename = list->currentItem()->text();
    emit mapselected(filename);
    dialog->accept();
}

QMapListTab::~QMapListTab()
{
}



QEditRefPointTab::QEditRefPointTab(QMapTabsDialog *d, QTabWidget* t, MapMetaData* m, RefPoint* r)
: QWidget(d), dialog(d), tab(t), name(0), position(0), x(0), y(0), meta(m)
, settings("karpeer.net","qTracker",this)
{
	QVBoxLayout* main = new QVBoxLayout();
	QGridLayout* gridbox = new QGridLayout();
	
	// Name ======================================
	name = new QLineEdit(this);
	
	QString prefix = r->Name();
	if (prefix!="ref")
	{
		name->setText(prefix);
		tab->addTab(this,"Edit");
	}
	else
	{
		name->setText(UniqueName(prefix));
		tab->addTab(this,"New" );
	}
	gridbox->addWidget(new QLabel(tr("Name:")),0,0);
	gridbox->addWidget(name,0,1);

	// Position =======================================
	geodata::Datum datum = (geodata::Datum) settings.value("map/datum",geodata::Wgs84_Geo).toInt();
    GeographicLib::GeoCoords p(r->Latitude(),r->Longitude());
    std::string pos;
    switch (datum)
    {
    	default:
    	case geodata::Wgs84_Geo: pos = p.GeoRepresentation();    break;
    	case geodata::Wgs84_DMS: pos = p.DMSRepresentation();    break;
    	case geodata::UTM_UTP:   pos = p.UTMUPSRepresentation(); break;
    	case geodata::MGRS:      pos = p.MGRSRepresentation();   break;
    }
	position = new QLineEdit(QString::fromStdString(pos));
	gridbox->addWidget(new QLabel(tr("Position:")),1,0);
	gridbox->addWidget(position,1,1);
	
	// Elevation ======================================
    x = new QDoubleEdit(r->X(),this);
	gridbox->addWidget(new QLabel(tr("X:")),3,0);
    gridbox->addWidget(x,3,1);
    
	// Time ======================================
    y = new QDoubleEdit(r->Y(),this);
	gridbox->addWidget(new QLabel(tr("Y:")),4,0);
    gridbox->addWidget(y,4,1);
    
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

QEditRefPointTab::~QEditRefPointTab()
{
}

void QEditRefPointTab::accept() 
{
    LOG( "QEditRefPointTab::accept()"; )
    if (!meta)
    {
        QMessageBox msg;
        msg.setText(tr("Unable to save refpoint."));
        msg.setIcon(QMessageBox::Warning);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        
        return;
    }
    
    GeographicLib::GeoCoords p(position->text().toStdString());
    RefPoint r = RefPoint(x->number(),y->number(),p.Latitude(),p.Longitude());
    meta->AddRefPoint(r);
    GpxIO::Instance()->WriteMapMetaFile(*meta);
}

double QEditRefPointTab::Latitude()
{
    GeographicLib::GeoCoords p(position->text().toStdString());
    return p.Latitude();
}

double QEditRefPointTab::Longitude()
{
    GeographicLib::GeoCoords p(position->text().toStdString());
    return p.Longitude();
}

void QEditRefPointTab::setvalue(const RefPoint& r)
{
	geodata::Datum datum = (geodata::Datum) settings.value("map/datum",geodata::Wgs84_Geo).toInt();
    GeographicLib::GeoCoords p(r.Latitude(),r.Longitude());
    std::string pos;
    switch (datum)
    {
    	default:
    	case geodata::Wgs84_Geo: pos = p.GeoRepresentation();    break;
    	case geodata::Wgs84_DMS: pos = p.DMSRepresentation();    break;
    	case geodata::UTM_UTP:   pos = p.UTMUPSRepresentation(); break;
    	case geodata::MGRS:      pos = p.MGRSRepresentation();   break;
    }
    position->setText(QString::fromStdString(pos));
	
    name->setText(r.Name());
    double ax = r.X();
    double ay = r.Y();
    x->setNumber(ax);
    y->setNumber(ay);
}



QDatumTab::QDatumTab(QMapTabsDialog *d, QTabWidget* t)
: QWidget(d), dialog(d), tab(t)
{
	QVBoxLayout* main = new QVBoxLayout();
	//QGridLayout* gridbox = new QGridLayout();
	
	tab->addTab(this,"Datum");
	
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
    //main->addLayout(gridbox);
    main->addWidget(filler);
    main->addLayout(buttonbox);
    setLayout(main);

	// Signals ======================================
    connect(cancel,SIGNAL(clicked()),d,SLOT(reject()));
    connect(confirm,SIGNAL(clicked()),this,SLOT(accept()));
    connect(confirm,SIGNAL(clicked()),d,SLOT(accept()));
}

QDatumTab::~QDatumTab()
{
}

void QDatumTab::accept() 
{
    LOG( "QDatumTab::accept()"; )
}

void QDatumTab::setvalue(geodata::Datum v)
{
}



QMapTabsDialog::QMapTabsDialog(QWidget *p, MapMetaData* m, RefPoint* r)
    : QDialog(p)
{
    QTabWidget* tabs = new QTabWidget(this);

    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);

    //new QCurrentMapTab(this,tabs);
    if (r)
        tabs->addTab(new QMapListTab(this,QMapListTab::CurrentMaps,r),tr("Local"));
    tabs->addTab(new QMapListTab(this,QMapListTab::AllMaps),tr("All"));
    new QEditRefPointTab(this,tabs,m,r);
    new QDatumTab(this,tabs);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
    showFullScreen();
    setAttribute(Qt::WA_DeleteOnClose);
}

void QMapTabsDialog::accept()
{
    QDialog::accept();
    close();
}
