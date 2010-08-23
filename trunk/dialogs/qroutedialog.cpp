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
