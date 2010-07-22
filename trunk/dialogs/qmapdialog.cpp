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

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG2( a ) std::cout << a
#define LOG( a )


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
    LOG( "QCurrentTrackTab::resizeEvent()\n"; )
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
    QPushButton* confirm = new QPushButton(tr("Confirm"));
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
    LOG( "QMapListTab::confirmselection()\n"; )
    QString filename = list->currentItem()->text();
    emit mapselected(filename);
    dialog->accept();
}

QMapListTab::~QMapListTab()
{
}



QMapTabsDialog::QMapTabsDialog(WayPoint* p, QWidget *parent)
    : QDialog(parent)
{
    QTabWidget* tabs = new QTabWidget(this);

    QFile file(CSSRCDIR "style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);

    //new QCurrentMapTab(this,tabs);
    if (p)
        tabs->addTab(new QMapListTab(this,QMapListTab::CurrentMaps,p),tr("Local"));
    //tabs->addTab(new QMapListTab(this,QMapListTab::RecentMaps),tr("Recent"));
    tabs->addTab(new QMapListTab(this,QMapListTab::AllMaps),tr("All"));
    tabs->addTab(new QWidget(this), tr("Options"));

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
