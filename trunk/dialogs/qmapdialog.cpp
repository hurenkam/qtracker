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
#include <QToolButton>
#include <QIcon>
#include <QList>
#include <QListWidgetItem>
#include "qmapdialog.h"
#include "maplist.h"
#include "datumlist.h"
#include "gpxio.h"

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
	//geodata::Datum datum = (geodata::Datum) settings.value("map/datum",geodata::Wgs84_Geo).toInt();
	position = new QLineEdit(DatumList::Instance().Representation(*r));
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
    
    WayPoint w = DatumList::Instance().Position(position->text());
    RefPoint r = RefPoint(x->number(),y->number(),w.Latitude(),w.Longitude());
    meta->AddRefPoint(r);
    GpxIO::Instance()->WriteMapMetaFile(*meta);
}

double QEditRefPointTab::Latitude()
{
    WayPoint w = DatumList::Instance().Position(position->text());
	return w.Latitude();
}

double QEditRefPointTab::Longitude()
{
    WayPoint w = DatumList::Instance().Position(position->text());
	return w.Longitude();
}

void QEditRefPointTab::setvalue(const RefPoint& r)
{
	//geodata::Datum datum = (geodata::Datum) settings.value("map/datum",geodata::Wgs84_Geo).toInt();
	position->setText(DatumList::Instance().Representation(r));
    name->setText(r.Name());
    double ax = r.X();
    double ay = r.Y();
    x->setNumber(ax);
    y->setNumber(ay);
}



QDatumTab::QDatumTab(QMapTabsDialog *d, QTabWidget* t)
: QWidget(d), dialog(d), tab(t)
, settings("karpeer.net","qTracker",this)
{
	QVBoxLayout* main = new QVBoxLayout();
	tab->addTab(this,"Datum");
	
    QGroupBox* distgroup = new QGroupBox();
    distgroup->setTitle("Datum");
    distbuttons = new QButtonGroup(distgroup);
    QVBoxLayout *distbox = new QVBoxLayout();
    datums = DatumList::Instance().Keys();
    datum = datums.indexOf(DatumList::Instance().GetDatum());
    for (int i=0; i<datums.length(); i++)
	{
        QRadioButton *button  = new QRadioButton(datums[i]);
        distbuttons->addButton(button,i);
        distbox->addWidget(button);
	}
    distgroup->setLayout(distbox);
    connect(distbuttons, SIGNAL(buttonClicked(int)),this,SLOT(setvalue(int)));

    
    
    towgsgroup = new QGroupBox();
    towgsgroup->setTitle("UTM"); 
	QGridLayout* towgsbox = new QGridLayout();
	// Ellipse =======================================
	towgsbox->addWidget(new QLabel(tr("Ellipsoid:")),1,0);
    list = new QComboBox();
    ellipses = DatumList::Instance().Ellipses();
    ellipse = ellipses.indexOf(Datum::utm->GetAttribute("ellipse"));
    list->addItems(ellipses);
    list->setCurrentIndex(ellipse);
    towgsbox->addWidget(list,1,1,1,3);
	
	// Translation ===================================
    double parms[7] = { 0,0,0,0,0,0,0 };
    QStringList strparms = Datum::utm->GetAttribute("towgs").split(",",QString::SkipEmptyParts);
    for (int i=0; i<strparms.length(); i++)
    	parms[i]=strparms[i].toDouble();
    	
    dx = new QDoubleEdit(parms[0],this);
	towgsbox->addWidget(new QLabel(tr("Dx:")),2,0);
	towgsbox->addWidget(dx,2,1);
    
    dy = new QDoubleEdit(parms[1],this);
    towgsbox->addWidget(new QLabel(tr("Dy:")),3,0);
    towgsbox->addWidget(dy,3,1);
    
    dz = new QDoubleEdit(parms[2],this);
    towgsbox->addWidget(new QLabel(tr("Dz:")),4,0);
    towgsbox->addWidget(dz,4,1);
    
	// Rotation ======================================
    rx = new QDoubleEdit(parms[3],this);
    towgsbox->addWidget(new QLabel(tr("Rx:")),2,2);
    towgsbox->addWidget(rx,2,3);
    
    ry = new QDoubleEdit(parms[4],this);
    towgsbox->addWidget(new QLabel(tr("Ry:")),3,2);
    towgsbox->addWidget(ry,3,3);
    
    rz = new QDoubleEdit(parms[5],this);
    towgsbox->addWidget(new QLabel(tr("Rz:")),4,2);
    towgsbox->addWidget(rz,4,3);
    
	// Scale ======================================
    m = new QDoubleEdit(parms[6],this);
    towgsbox->addWidget(new QLabel(tr("M:")),5,0);
    towgsbox->addWidget(m,5,1);
    towgsgroup->setLayout(towgsbox);

    
    
    // Filler ======================================
    filler = new QWidget;
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
	// Buttons ======================================
	QPushButton* confirm =  new QPushButton(tr("Save"));
	QPushButton* cancel  =  new QPushButton(tr("Cancel"));
	QHBoxLayout* buttonbox =  new QHBoxLayout();
	buttonbox->addWidget(confirm);
	buttonbox->addWidget(cancel);

	// Layout ======================================
    center = new QBoxLayout(QBoxLayout::LeftToRight);
    center->addWidget(distgroup);
    center->addWidget(towgsgroup);
    main->addLayout(center);
    main->addWidget(filler);
    main->addLayout(buttonbox);
    setLayout(main);

	// Signals ======================================
    connect(cancel,SIGNAL(clicked()),d,SLOT(reject()));
    connect(confirm,SIGNAL(clicked()),this,SLOT(accept()));
    connect(confirm,SIGNAL(clicked()),d,SLOT(accept()));
    
    setvalue(datum);
    //setellipse(ellipse);
}

QDatumTab::~QDatumTab()
{
}

void QDatumTab::resizeEvent( QResizeEvent * event )
{
    if (!center) return;
    
    if (event->size().width() < event->size().height())
        center->setDirection(QBoxLayout::TopToBottom);
    else
        center->setDirection(QBoxLayout::LeftToRight);

    QWidget::resizeEvent(event);
}

void QDatumTab::accept() 
{
    LOG( "QDatumTab::accept()"; )
    DatumList::Instance().SetDatum(datums[datum]);
    
    if (datums[datum]=="UTM")
    {
    	QString towgs;
    	towgs.sprintf(
    			"%f,%f,%f,%f,%f,%f,%f",
    			dx->number(),dy->number(),dz->number(),
    			rx->number(),ry->number(),rz->number(),
    			m->number()
    		);
        Datum::utm->SetAttribute("ellipse",ellipses[list->currentIndex()]);
        Datum::utm->SetAttribute("towgs",towgs);
    }
}

void QDatumTab::setvalue(int v)
{
	distbuttons->button(v)->setChecked(true);
	datum = v;
	if (datums[v]=="UTM")
	{
		towgsgroup->show();
	}
	else
    {
		towgsgroup->hide();
    }
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
    move(0,0);
    setModal(true);
    showFullScreen();
    setAttribute(Qt::WA_DeleteOnClose);
}

void QMapTabsDialog::accept()
{
    QDialog::accept();
    close();
}
